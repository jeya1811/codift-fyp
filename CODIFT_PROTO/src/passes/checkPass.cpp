#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include <set>
#include <vector>

using namespace llvm;

namespace {
  class SelectiveCodiftCheckPass : public FunctionPass {
    public:
      static char ID;
      SelectiveCodiftCheckPass() : FunctionPass(ID) {}

      bool runOnFunction(Function& F) override {
        if (!isSecurityRelevantFunction(F))
          return false;

        bool modified = false;
        std::vector<Instruction*> WorkList;

        // Scan phase
        for (auto& BB : F) {
          for (auto& I : BB) {
            if (auto* BI = dyn_cast<BranchInst>(&I)) {
              if (BI->isConditional() && mightBeTainted(BI->getCondition(), &F))
                WorkList.push_back(&I);
            } else if (auto* CI = dyn_cast<CallInst>(&I)) {
              Function* calledFunc = CI->getCalledFunction();
              if (calledFunc && isSecuritySink(calledFunc))
                WorkList.push_back(&I);
            } else if (auto* RI = dyn_cast<ReturnInst>(&I)) {
              if (RI->getReturnValue() && mightBeTainted(RI->getReturnValue(), &F))
                WorkList.push_back(&I);
            }
          }
        }

        // Injection phase
        for (Instruction* I : WorkList) {
          if (auto* BI = dyn_cast<BranchInst>(I)) {
            injectSecurityCheck(BI->getCondition(), I, &F);
            modified = true;
          } else if (auto* CI = dyn_cast<CallInst>(I)) {
            for (unsigned i = 0; i < CI->arg_size(); ++i) {
              if (mightBeTainted(CI->getArgOperand(i), &F)) {
                injectSecurityCheck(CI->getArgOperand(i), I, &F);
                modified = true;
              }
            }
          } else if (auto* RI = dyn_cast<ReturnInst>(I)) {
            injectSecurityCheck(RI->getReturnValue(), I, &F);
            modified = true;
          }
        }

        return modified;
      }

    private:
      // ============== Security Policy ==============

      bool isSecurityRelevantFunction(Function& F) {
        StringRef name = F.getName();
        std::set<std::string> securityFunctions = {
          "verify_pin",
          "activate_lock",
          "grant_access",
          "sound_alarm",
          "display_access_message",
          "check_authentication",
          "main"
        };
        return securityFunctions.count(name.str()) > 0;
      }

      bool isSecuritySink(Function* F) {
        if (!F)
          return false;
        StringRef name = F->getName();
        return name.contains("lock") ||
               name.contains("grant") ||
               name.contains("alarm") ||
               name.contains("access") ||
               name.contains("authenticate");
      }

      // ============== Taint Analysis ==============

      bool mightBeTainted(Value* V, Function* F) {
        if (isa<Constant>(V))
          return false;

        if (auto* LI = dyn_cast<LoadInst>(V))
          return true;

        if (auto* CI = dyn_cast<CallInst>(V)) {
          Function* calledFunc = CI->getCalledFunction();
          if (calledFunc && isUntrustedSource(calledFunc))
            return true;
        }

        if (auto* I = dyn_cast<Instruction>(V)) {
          for (auto& Op : I->operands()) {
            if (mightBeTainted(Op.get(), F))
              return true;
          }
        }

        return false;
      }

      bool isUntrustedSource(Function* F) {
        if (!F)
          return false;
        StringRef name = F->getName();
        return name.contains("keypad") ||
               name.contains("read") ||
               name.contains("input") ||
               name.contains("serial") ||
               name.contains("network");
      }

      // ============== Check Injection ==============

      void injectSecurityCheck(Value* valueToCheck, Instruction* location, Function* parentFunc) {
        IRBuilder<> Builder(location);
        FunctionCallee readFunc = getOrCreateReadFunc(parentFunc);
        FunctionCallee secFunc = getOrCreateSecFunc(parentFunc);

        Value* valPtr = getSafePtr(valueToCheck, Builder, parentFunc);
        Value* tag = Builder.CreateCall(readFunc, {valPtr});
        Builder.CreateCall(secFunc, {tag});
      }

      FunctionCallee getOrCreateReadFunc(Function* F) {
        Module* M = F->getParent();
        LLVMContext& Context = M->getContext();
        FunctionType* readFT = FunctionType::get(Type::getInt8Ty(Context), {Type::getInt8PtrTy(Context)}, false);
        return M->getOrInsertFunction("ramReadFunc", readFT);
      }

      FunctionCallee getOrCreateSecFunc(Function* F) {
        Module* M = F->getParent();
        LLVMContext& Context = M->getContext();
        FunctionType* secFT = FunctionType::get(Type::getVoidTy(Context), {Type::getInt8Ty(Context)}, false);
        return M->getOrInsertFunction("secExcFunc", secFT);
      }

      // Dominance-safe pointer retrieval
      Value* getSafePtr(Value* V, IRBuilder<>& B, Function* F) {
        if (V->getType()->isPointerTy())
          return B.CreatePointerCast(V, Type::getInt8PtrTy(F->getContext()));

        BasicBlock& Entry = F->getEntryBlock();
        IRBuilder<> TmpB(&Entry, Entry.getFirstInsertionPt());
        AllocaInst* A = TmpB.CreateAlloca(V->getType(), nullptr, "codift_chk_tmp");

        if (Instruction* I = dyn_cast<Instruction>(V)) {
          IRBuilder<> StoreB(I->getNextNode());
          StoreB.CreateStore(V, A);
        } else {
          B.CreateStore(V, A);
        }

        return B.CreatePointerCast(A, Type::getInt8PtrTy(F->getContext()));
      }
  };
} // namespace

char SelectiveCodiftCheckPass::ID = 1;
static RegisterPass<SelectiveCodiftCheckPass>
  X("check", "Selective CODIFT Check Pass");