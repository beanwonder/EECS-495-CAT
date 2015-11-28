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
#include "llvm/IR/CFG.h"
#include "llvm/IR/Type.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include <iostream>
#include <map>
#include <vector>
#include <assert.h>
#include <set>
#include <algorithm>
#include <iterator>
using namespace llvm;

namespace {
  struct CatPass : public FunctionPass {
    static char ID; 

    const static uint32_t CAT_binary_add_ID = 0,
                          CAT_create_signed_value_ID = 1,
                          CAT_get_signed_value_ID = 2;
                          //CAT_fake_instruction_ID = 3;
    const std::vector<std::string> CAT_function_names = {"CAT_binary_add", "CAT_create_signed_value", "CAT_get_signed_value"};
    const std::vector<uint32_t> CAT_function_IDS = {CAT_binary_add_ID, CAT_create_signed_value_ID, CAT_get_signed_value_ID};
    std::map<Function *, std::string> CAT_functions;

    CatPass() : FunctionPass(ID) {}
   
    // This function is invoked once at the initialization phase of the compiler
    // The LLVM IR of functions isn't ready at this point
    bool doInitialization(Module &M) override {
      bool isFound = false;
      for (int i = 0; i < CAT_function_names.size(); ++i) {
        Function *CAT_function;
        CAT_function = M.getFunction(CAT_function_names[i].c_str());
        if (CAT_function != NULL) {
          isFound = true;
        }
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

      Constant *zeroConst = ConstantInt::get(IntegerType::get(F.getContext(), 32), 0, true);
      // first insert a fake node
      //auto argList = F.getArgumentList();
      auto first_intertion_pt = F.getEntryBlock().getFirstInsertionPt();
      std::map<Instruction *, Value *> fake_insts_to_arg;
      std::map<Value *, Instruction *> arg_to_fake_insts;
      for (auto iter = F.arg_begin(); iter != F.arg_end(); ++iter) {
        Instruction *newInst = BinaryOperator::Create(Instruction::Add, zeroConst, 
                                                      zeroConst, "", first_intertion_pt);
        fake_insts_to_arg[newInst] = iter;
        arg_to_fake_insts[iter] = newInst;
      }

      for (auto &B : F) {
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

          // KILL AND GENS
          if (!setCalleeID(&I, &calleeID, &callInst)) {
            if (fake_insts_to_arg.find(&I) != fake_insts_to_arg.end()) {
              GEN_sets.back().insert(&I);
            }
            continue;
          }         
          // normal GEN KILL SETS
          // if not a CAT_function no GEN KILL
          // whether has fucntion argument in add first operand
          // init
          switch (calleeID) {
            case CAT_binary_add_ID: {
              // GEN set
              GEN_sets.back().insert(&I);
              // KILL SET
              Value* defineVal = callInst->getArgOperand(0);
              if (isa<Argument>(defineVal)) {
                KILL_sets.back().insert(arg_to_fake_insts[defineVal]);
              }
              KILL_sets.back().insert(dyn_cast<Instruction>(defineVal));
              for (auto iter = defineVal->use_begin(); iter != defineVal->use_end(); ++iter) {
                  uint32_t cid;
                  Instruction *val_user_inst = dyn_cast<Instruction>(iter->getUser());
                  CallInst *cinst;
                  if (setCalleeID(val_user_inst, &cid, &cinst)) {
                    if (cid == CAT_binary_add_ID) {
                      // related to h5-2 definitions should be treated equally
                      if (defineVal ==  cinst->getArgOperand(0)) {
                        KILL_sets.back().insert(val_user_inst);
                      }                  
                    }
                  }
              }

              /*
              auto search = KILL_sets.back().find(&I);
              if (search != KILL_sets.back().end()) {
                KILL_sets.back().erase(search);
              }
              */
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

      std::vector<std::set<Instruction *>> IN_sets;
      std::vector<std::set<Instruction *>> OUT_sets;
      std::map<Instruction *, int> inst_to_idx;

      for (int idx = 0; idx < inst_index.size(); ++idx) {
        inst_to_idx[inst_index[idx]] = idx;
      }
      // init in/out sets
      std::set<Instruction *> entry_set;
      for (int i = 0; i < inst_index.size(); ++i) {
        IN_sets.push_back(std::set<Instruction *>());
        OUT_sets.push_back(std::set<Instruction *>());
      }
      // compute IN and OUT set for each instruciton
      bool is_changed;
      do
      {
        is_changed = false;
        for (auto &B : F) {
          std::set<Instruction *> bb_in;
          for (pred_iterator PI = pred_begin(&B), E = pred_end(&B); PI != E; ++PI) {
            auto p_out = OUT_sets[inst_to_idx[(*PI)->getTerminator()]];
            // union one predecessor
            bb_in.insert(p_out.begin(), p_out.end());
          }
          // compute the first inst in out
          //errs() << inst_to_idx.at(&(B.front())) << '\n';
          int fid = inst_to_idx[&(B.front())];
          IN_sets[fid] = bb_in;

          // IN - KILL
          std::set<Instruction *> diff;
          std::set_difference(IN_sets[fid].begin(), IN_sets[fid].end(),
                              KILL_sets[fid].begin(), KILL_sets[fid].end(),
                              std::inserter(diff, diff.end()));
          std::set<Instruction *> tmp_out;
          tmp_out.insert(GEN_sets[fid].begin(), GEN_sets[fid].end());
          tmp_out.insert(diff.begin(), diff.end());
          if (tmp_out != OUT_sets[fid]) {
            OUT_sets[fid] = tmp_out;
            is_changed = true;
          }
          // compute rest in out sets
          for (BasicBlock::iterator iter = ++B.begin(); iter != B.end(); ++iter) {
            int idx = inst_to_idx[&(*iter)];
            IN_sets[idx] = OUT_sets[idx-1];
            std::set<Instruction *> diff;
            std::set_difference(IN_sets[idx].begin(), IN_sets[idx].end(),
                                KILL_sets[idx].begin(), KILL_sets[idx].end(),
                                std::inserter(diff, diff.end()));
            std::set<Instruction *> tmp_out;
            tmp_out.insert(GEN_sets[idx].begin(), GEN_sets[idx].end());
            tmp_out.insert(diff.begin(), diff.end());

            if (tmp_out != OUT_sets[idx]) {
              OUT_sets[idx] = tmp_out;
              is_changed = true;
            }

          }
        }
      } while (is_changed);

      // reaching definition
      for (int idx = 0; idx < inst_index.size(); ++idx) {
        Instruction *inst = inst_index[idx];
        uint32_t callid;
        CallInst *callinst;
        if (setCalleeID(inst, &callid, &callinst)) {
          if (callid == CAT_get_signed_value_ID) {
            Value *var = callinst->getArgOperand(0);
            Instruction *var_inst = dyn_cast<Instruction>(var);
            std::set<Instruction *> IN = IN_sets[idx];
            Instruction *reachingDef = NULL;
            // check IN set elements
            for (std::set<Instruction *>::iterator iter = IN.begin(); iter != IN.end(); ++iter) {
              bool done = false;
              uint32_t id;
              CallInst *cinst;
              if (!setCalleeID(*iter, &id, &cinst)) {
                if (fake_insts_to_arg.find(*iter) != fake_insts_to_arg.end()) {
                  // if iter is fake_instruction
                  //errs() << "i am fake inst\n";
                  if (fake_insts_to_arg[*iter] == var) {
                    //errs() << "i am killing var\n";
                    reachingDef = NULL;
                    break;
                  } else {
                    // not match
                    // do nothing
                    continue;
                  }
                } else {
                  continue;
                }

              } else {
                switch (id) {
                  case CAT_create_signed_value_ID: {
                    if (var_inst == *iter) {
                      reachingDef = *iter;
                    }
                    break;
                  }
                  case CAT_binary_add_ID: {
                    if (var == cinst->getArgOperand(0)) {
                      reachingDef = NULL;
                      done = true;
                    }
                    break;
                  }
                  case CAT_get_signed_value_ID: {
                    break;
                  }
                }
                if (done) {
                  break;
                }  
              }  
            }
            // constant propagate
            //int64_t c;
            if (reachingDef != NULL) {
              CallInst *v_to_c = dyn_cast<CallInst>(reachingDef);
              Value *v = v_to_c->getArgOperand(0);
              if (isa<ConstantInt>(v)) {
                ConstantInt *cnst = dyn_cast<ConstantInt>(v);
                BasicBlock::iterator ii(inst);
                ReplaceInstWithValue(inst->getParent()->getInstList(), ii, cnst);
              }
            }
          }
        }

      }

      // finally delete all fake instructions
      for (std::map<Instruction *, Value *>::iterator iter = fake_insts_to_arg.begin(); iter != fake_insts_to_arg.end(); ++iter) {
        (iter->first)->eraseFromParent();
      }
      return true;
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