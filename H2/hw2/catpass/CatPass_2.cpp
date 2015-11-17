#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include <iostream>
#include <map>
#include <vector>
#include <assert.h>
#include <set>
using namespace llvm;

namespace {
  struct CatPass : public FunctionPass {
    static char ID; 

    const static uint32_t CAT_binary_add_ID = 0,
                          CAT_create_signed_value_ID = 1,
                          CAT_get_signed_value_ID = 2;
    const std::vector<std::string> CAT_function_names = {"CAT_binary_add", "CAT_create_signed_value", "CAT_get_signed_value"};
    const std::vector<uint32_t> CAT_function_IDS = {CAT_binary_add_ID, CAT_create_signed_value_ID, CAT_get_signed_value_ID};
    std::map<Function *, std::string> CAT_functions;

    CatPass() : FunctionPass(ID) {}
   
    // This function is invoked once at the initialization phase of the compiler
    // The LLVM IR of functions isn't ready at this point
    bool doInitialization(Module &M) override {
      //errs() << "Hello LLVM World at \"doInitialization\"\n" ;
      bool isFound = false;
      for (int i = 0; i < CAT_function_names.size(); ++i) {
        Function *CAT_function;
        CAT_function = M.getFunction(CAT_function_names[i].c_str());
        if (CAT_function != NULL) {
          isFound = true;
          //errs() << "CAT ERROR CAT function couldn't be found\n";
          //abort();
        }
        //errs() << "Functions: " << *CAT_function << '\n';
        CAT_functions[CAT_function] = CAT_function_names[i];
      }

      if (!isFound) {
        errs() << "CAT ERROR CAT Function coudln't be found\n";
        abort();
      }

      return isFound;
    }

    // This function is invoked once per function compiled
    // The LLVM IR of the input functions is ready and it can be analyzed and/or transformed
    virtual bool runOnFunction(Function &F) {
      //errs() << "Hello LLVM World at \"getAnalysisUsage\"\n" ;
      // Instruction are 
      std::vector<Instruction *> inst_index;
      std::vector<std::set<Instruction *>> GEN_sets;
      std::vector<std::set<Instruction *>> KILL_sets;
      //GEN and KILL is based on each instruction's basic block
      for (auto &B : F) {
        // iterate over all basic blocks
        //errs() << "block begin" << '\n';
        for (auto &I : B) {        
          // interate over instructions in a basic block
          //errs() << I << '\n';
          CallInst *callInst;
          uint32_t calleeID;
          // whether is a cat function call
          //init kill gen set for each Instruction
          inst_index.push_back(&I);          
          GEN_sets.push_back(std::set<Instruction *>());
          KILL_sets.push_back(std::set<Instruction *>());
          if (!setCalleeID(&I, &calleeID, &callInst)) {
            continue;
          }         
          // GEN KILL SETS
          // if not a CAT_function no GEN KILL
          // empty set is assmumed
          // init
          switch (calleeID) {
            case CAT_binary_add_ID: {
              // GEN set
              GEN_sets.back().insert(&I);
              // KILL SET
              Value* defineVal = callInst->getArgOperand(0);
              KILL_sets.back().insert(dyn_cast<Instruction>(defineVal));
              for (auto iter = defineVal->use_begin(); iter != defineVal->use_end(); ++iter) {
                  uint32_t cid;
                  Instruction *val_user_inst = dyn_cast<Instruction>(iter->getUser());
                  CallInst *cinst;
                  if (setCalleeID(val_user_inst, &cid, &cinst)) {
                    if (cid == CAT_binary_add_ID) {
                      if (defineVal ==  cinst->getArgOperand(0)) {
                        KILL_sets.back().insert(val_user_inst);
                      }                    }
                  }
              }
              auto search = KILL_sets.back().find(&I);
              if (search != KILL_sets.back().end()) {
                KILL_sets.back().erase(search);
              }
              break;
            }
            case CAT_create_signed_value_ID: {
              /*add current instruction to GEN set, no kill
              */
              GEN_sets.back().insert(&I);
              break;
            }
            case CAT_get_signed_value_ID: {
              break;
            }
          }

        }
        //errs() << "block end" << '\n';
      }

      errs() << "START FUNCTION: " << F.getName() << '\n';
      for (uint32_t idx = 0; idx < inst_index.size(); ++idx) {
        Instruction &I = *inst_index[idx];
        errs() << "INSTRUCTION: " << I << '\n';
        errs() << "***************** GEN\n";
        errs() << "{\n";
        if (!GEN_sets[idx].empty()) {
          for (auto inst = GEN_sets[idx].begin(); inst != GEN_sets[idx].end(); ++inst) {
            errs() << ' ' << **inst << '\n';
          }
        }
        errs() << "}\n";
        errs() << "**************************************\n";
        errs() << "***************** KILL\n";
        errs() << "{\n";
        if (!KILL_sets[idx].empty()) {
          for (auto inst = KILL_sets[idx].begin(); inst != KILL_sets[idx].end(); ++inst) {
            errs() << ' ' << **inst << '\n';
          }
        }
        errs() << "}\n";
        errs() << "**************************************\n";
        errs() << "\n\n\n";

      }

      return false;
    }

    // We don't modify the program, so we preserve all analyses.
    // The LLVM IR of functions isn't ready at this point
    virtual void getAnalysisUsage(AnalysisUsage &AU) const {
      //errs() << "Hello LLVM World at \"getAnalysisUsage\"\n" ;
      AU.setPreservesAll();
    }

   private: 
    bool setCalleeID(Instruction *i, uint32_t *calleeID, CallInst **callInst) {
      Function *callee;
      std::string calleeName;
      // call?
      if (!isa<CallInst>(i)) {
        return false;
      }
      // CAT_function call?
      (*callInst) = cast<CallInst>(i);
      //errs() << "Function: " << *callee << '\n';
      callee = (*callInst)->getCalledFunction();
      if (CAT_functions.find(callee) != CAT_functions.end()) {
        calleeName = CAT_functions[callee];
      }

      if (calleeName.empty()) {
        return false;
      }
      // set calleeID
      (*calleeID) = find(CAT_function_names.begin(), CAT_function_names.end(), calleeName) - CAT_function_names.begin();
      return true;
    }
  };
}


// Next there is code to register your pass to the LLVM compiler
char CatPass::ID = 0;

static void registerCatPass (const PassManagerBuilder &, legacy::PassManagerBase &PM) {
  PM.add(new CatPass());
}

static RegisterStandardPasses RegisterMyPass (PassManagerBuilder::EP_EarlyAsPossible, registerCatPass);
static RegisterPass<CatPass> X("CAT", "A pass built for the CAT class at Northwestern");
