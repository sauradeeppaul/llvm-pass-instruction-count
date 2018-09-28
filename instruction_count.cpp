#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
using namespace llvm;

unsigned int tc = 0;
unsigned int tmc = 0;
unsigned int tbc = 0;
unsigned int tac = 0;

namespace {
  struct SkeletonPass : public FunctionPass {
    static char ID;
    SkeletonPass() : FunctionPass(ID) {}

    virtual bool runOnFunction(Function &F) {
      
      unsigned int inst_count = 0;
      unsigned int mem_inst_count = 0;
      unsigned int branch_inst_count = 0;
      unsigned int math_inst_count = 0;

      for (auto &Block : F) {
        for (auto &I : Block) {
          inst_count += 1;
          if(I.mayReadOrWriteMemory()){
             mem_inst_count += 1;
          }
          if(isa<BranchInst>(I)){
             branch_inst_count += 1;
          }
          if(isa<BinaryOperator>(I)){
             math_inst_count += 1;
          }
        }
      }
      llvm::outs() << "Counts for " << F.getName() << "\n";
      llvm::outs() << "Instructions Count: " << inst_count << "\n";
      llvm::outs() << "Memory Access Instructions Count: " << mem_inst_count << "\n";
      llvm::outs() << "Branch Instructions Count: " << branch_inst_count << "\n";
      llvm::outs() << "Arithmetic Instructions Count: "<<math_inst_count << "\n";
      
      tc += inst_count;
      tmc += mem_inst_count;
      tbc += branch_inst_count;
      tac += math_inst_count;
      
      llvm::outs() << "\n Count till now:\n";
      llvm::outs() << "Total Instruction Count: "<< tc << "\n";
      llvm::outs() << "Total Memory Access Count: "<< tmc << "\n";
      llvm::outs() << "Total Branch Instructions Count: " << tbc << "\n";
      llvm::outs() << "Total Arithmetic Instructions Count: "<< tac << "\n";

     return false;
    }
  };
}

char SkeletonPass::ID = 0;

// Automatically enable the pass.
// http://adriansampson.net/blog/clangpass.html
static void registerSkeletonPass(const PassManagerBuilder &,
                         legacy::PassManagerBase &PM) {
  PM.add(new SkeletonPass());
  }
static RegisterStandardPasses
  RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
                 registerSkeletonPass);
