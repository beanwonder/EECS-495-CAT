#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>

using namespace llvm;
using namespace std;

namespace {
  struct CatPass : public FunctionPass {
    static char ID; 
    CatPass() : FunctionPass(ID) {}

    // This function is invoked once at the initialization phase of the compiler
    // The LLVM IR of functions isn't ready at this point
    bool doInitialization(Module &M) {
      return false;
    }

    // This function is invoked once per function compiled
    // The LLVM IR of the input functions is ready and it can be analyzed and/or transformed
    virtual bool runOnFunction(Function &F) {
      // errs() << "Function " << "\"" + F.getName() + "\"\n";
      // F.print(errs()); 
      if (F.getName().str() == "main")
        return false;

      map<StringRef, uint32_t> function_counter;
      for (auto& B : F) {
	for (auto& I : B) {
	  if (auto* call = dyn_cast<CallInst>(&I)) {
	    Function *callee;
	    callee = call->getCalledFunction();
	    ++function_counter[callee->getName()];
  	  }	    
	}
      }
      // output
      string file_name = "compiler_output";
      ofstream out(file_name, ofstream::out | ofstream::app);
      for (auto& pair : function_counter) {
	if (pair.first.str().compare(0, 3, "CAT") == 0) {
	  out << "H1: \"" << F.getName().str() << "\": " << pair.first.str() << ": " << pair.second << endl;
	}
      }
      out.close();
      return false;
    }

    // We don't modify the program, so we preserve all analyses.
    // The LLVM IR of functions isn't ready at this point
    virtual void getAnalysisUsage(AnalysisUsage &AU) const {
      AU.setPreservesAll();
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
