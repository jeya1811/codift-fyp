#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include <set>
#include <vector>
using namespace llvm;

namespace {
  struct CheckPass : public FunctionPass {
      static char ID;
      CheckPass() : FunctionPass(ID) {}

      bool runOnFunction(Function& F) override {
        static const std::set<std::string> WL = {
          "read_input", "verify", "main"
        };
        if (!WL.count(F.getName().str()))
          return false;
        errs() << "[Check]  " << F.getName() << "\n";

        Module* M = F.getParent();

        GlobalVariable* sGV = nullptr;
        for (auto& G : M->globals()) {
          StringRef n = G.getName();
          if (n == "s" || n.startswith("s.")) {
            sGV = &G;
            break;
          }
        }
        if (!sGV) {
          errs() << "[Check]  WARNING: @s not found\n";
          return false;
        }
        errs() << "[Check]  found: " << sGV->getName() << "\n";

        std::vector<Instruction*> targets;
        for (auto& BB : F)
          for (auto& I : BB) {
            auto* LI = dyn_cast<LoadInst>(&I);
            if (!LI)
              continue;
            if (isVerifiedPtr(LI->getPointerOperand(), sGV))
              targets.push_back(&I);
          }
        if (targets.empty())
          return false;

        errs() << "[Check]  " << targets.size()
               << " s.verified load(s) in " << F.getName() << " — injecting\n";
        for (auto* I : targets)
          inject(sGV, I, M);
        return true;
      }

    private:
      bool isVerifiedPtr(Value* ptr, GlobalVariable* sGV) {
        if (!ptr)
          return false;
        if (auto* GEP = dyn_cast<GetElementPtrInst>(ptr)) {
          if (!tracesTo(GEP->getPointerOperand(), sGV))
            return false;
          if (GEP->getNumIndices() >= 2) {
            if (auto* CI = dyn_cast<ConstantInt>(
                  GEP->getOperand(GEP->getNumOperands() - 1)
                ))
              if (CI->getZExtValue() == 1)
                return true;
          }
          if (GEP->getNumIndices() == 1) {
            if (auto* CI = dyn_cast<ConstantInt>(GEP->getOperand(1)))
              if (CI->getZExtValue() == 4)
                return true;
          }
        }
        if (auto* BC = dyn_cast<BitCastInst>(ptr))
          return isVerifiedPtr(BC->getOperand(0), sGV);
        if (auto* CE = dyn_cast<ConstantExpr>(ptr)) {
          unsigned op = CE->getOpcode();
          if (op == Instruction::GetElementPtr) {
            if (!tracesTo(CE->getOperand(0), sGV))
              return false;
            if (CE->getNumOperands() >= 3) {
              if (auto* CI = dyn_cast<ConstantInt>(
                    CE->getOperand(CE->getNumOperands() - 1)
                  ))
                if (CI->getZExtValue() == 1 || CI->getZExtValue() == 4)
                  return true;
            }
          }
          if (op == Instruction::BitCast)
            return isVerifiedPtr(CE->getOperand(0), sGV);
        }
        return false;
      }

      bool tracesTo(Value* v, GlobalVariable* gv) {
        if (!v)
          return false;
        if (auto* G = dyn_cast<GlobalVariable>(v))
          return G == gv;
        if (auto* G = dyn_cast<GetElementPtrInst>(v))
          return tracesTo(G->getPointerOperand(), gv);
        if (auto* B = dyn_cast<BitCastInst>(v))
          return tracesTo(B->getOperand(0), gv);
        if (auto* C = dyn_cast<ConstantExpr>(v)) {
          unsigned op = C->getOpcode();
          if (op == Instruction::BitCast || op == Instruction::GetElementPtr ||
              op == Instruction::AddrSpaceCast)
            return tracesTo(C->getOperand(0), gv);
        }
        return false;
      }

      void inject(GlobalVariable* gv, Instruction* before, Module* M) {
        IRBuilder<> B(before);
        LLVMContext& C = M->getContext();
        Type* i8 = Type::getInt8Ty(C);
        Type* i8p = PointerType::getUnqual(i8);

        Value* base = B.CreateBitCast(gv, i8p, "cd.base");
        Value* off = ConstantInt::get(Type::getInt32Ty(C), 4);
        Value* vptr = B.CreateGEP(i8, base, off, "cd.vptr");

        FunctionCallee rf = M->getOrInsertFunction("ramReadFunc", FunctionType::get(i8, {i8p}, false));
        Value* tag = B.CreateCall(rf, {vptr}, "cd.tag");

        FunctionCallee sf = M->getOrInsertFunction("secExcFunc", FunctionType::get(Type::getVoidTy(C), {i8, i8p}, false));
        B.CreateCall(sf, {tag, vptr});

        errs() << "[Check]  -> injected secExcFunc before s.verified load in "
               << before->getFunction()->getName() << "\n";
      }
  };
} // namespace
char CheckPass::ID = 1;
static RegisterPass<CheckPass> X("check", "CO-DIFT Check", false, false);
