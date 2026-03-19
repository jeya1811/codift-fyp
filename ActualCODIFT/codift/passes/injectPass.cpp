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
struct InjectPass : public FunctionPass {
  static char ID;
  InjectPass() : FunctionPass(ID) {}

  bool runOnFunction(Function& F) override {
    static const std::set<std::string> WL = {
        "read_input", "verify", "main"};
    if (!WL.count(F.getName().str()))
      return false;
    errs() << "[Inject] " << F.getName() << "\n";

    Module* M = F.getParent();
    bool isRI = (F.getName() == "read_input");

    if (isRI) {
      Instruction* first = &*F.getEntryBlock().getFirstInsertionPt();
      IRBuilder<> B(first);
      FunctionCallee initFn = M->getOrInsertFunction("codift_init",
                                                     FunctionType::get(Type::getVoidTy(M->getContext()), {}, false));
      B.CreateCall(initFn, {});
      errs() << "[Inject]  auto-injected codift_init() at entry\n";
    }

    GlobalVariable* sGV = nullptr;
    for (auto& G : M->globals()) {
      StringRef n = G.getName();
      if (n == "s" || n.startswith("s.")) {
        sGV = &G;
        break;
      }
    }

    std::vector<Instruction*> work;
    for (auto& BB : F)
      for (auto& I : BB) {
        if (auto* SI = dyn_cast<StoreInst>(&I)) {
          if (isa<Constant>(SI->getValueOperand()))
            continue;
          if (isRI) {
            if (SI->getPointerOperand()->getType()->isPointerTy())
              work.push_back(&I);
          } else if (sGV) {
            if (tracesTo(SI->getPointerOperand(), sGV))
              work.push_back(&I);
          }
        } else if (auto* BO = dyn_cast<BinaryOperator>(&I)) {
          if (BO->getOpcode() == Instruction::Xor &&
              !isa<Constant>(BO->getOperand(0)))
            work.push_back(&I);
        }
      }

    bool changed = isRI;
    int sc = 0, xc = 0;
    for (auto* I : work) {
      if (auto* SI = dyn_cast<StoreInst>(I)) {
        if (tagStore(SI, M)) {
          changed = true;
          sc++;
        }
      } else if (auto* BO = dyn_cast<BinaryOperator>(I)) {
        if (propXor(BO, M)) {
          changed = true;
          xc++;
        }
      }
    }
    if (sc || xc)
      errs() << "[Inject]  " << sc << " stores " << xc << " xors\n";
    return changed;
  }

  private:
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
  bool tagStore(StoreInst* SI, Module* M) {
    Instruction* nx = SI->getNextNode();
    if (!nx)
      return false;
    IRBuilder<> B(nx);
    LLVMContext& C = M->getContext();
    Type *i8 = Type::getInt8Ty(C), *i8p = PointerType::getUnqual(i8);
    Value* d = B.CreateBitCast(SI->getPointerOperand(), i8p, "cd.p");
    FunctionCallee wf = M->getOrInsertFunction("ramWriteFunc",
                                               FunctionType::get(Type::getVoidTy(C), {i8p, i8}, false));
    B.CreateCall(wf, {d, B.getInt8(1)});
    return true;
  }
  bool propXor(BinaryOperator* BO, Module* M) {
    Instruction* nx = BO->getNextNode();
    if (!nx)
      return false;
    if (isa<Constant>(BO->getOperand(0)) && isa<Constant>(BO->getOperand(1)))
      return false;
    IRBuilder<> B(nx);
    LLVMContext& C = M->getContext();
    Type *i8 = Type::getInt8Ty(C), *i8p = PointerType::getUnqual(i8);
    FunctionCallee rf = M->getOrInsertFunction("ramReadFunc",
                                               FunctionType::get(i8, {i8p}, false));
    FunctionCallee wf = M->getOrInsertFunction("ramWriteFunc",
                                               FunctionType::get(Type::getVoidTy(C), {i8p, i8}, false));
    Value* t1 = getTag(BO->getOperand(0), B, rf);
    Value* t2 = getTag(BO->getOperand(1), B, rf);
    Value* mg = B.CreateOr(t1, t2, "cd.m");
    Value* rp = resPtr(BO, B);
    if (!rp)
      return false;
    B.CreateCall(wf, {rp, mg});
    return true;
  }
  Value* getTag(Value* V, IRBuilder<>& B, FunctionCallee rf) {
    if (!V || isa<Constant>(V))
      return B.getInt8(0);
    Value* p = valPtr(V, B);
    if (!p)
      return B.getInt8(0);
    return B.CreateCall(rf, {p}, "cd.t");
  }
  Value* valPtr(Value* V, IRBuilder<>& B) {
    if (!V)
      return nullptr;
    LLVMContext& C = B.getContext();
    Type* i8p = PointerType::getUnqual(Type::getInt8Ty(C));
    if (V->getType()->isPointerTy())
      return B.CreateBitCast(V, i8p, "cd.vp");
    if (isa<PHINode>(V) || V->getType()->isVoidTy())
      return nullptr;
    Function* F = nullptr;
    if (auto* I = dyn_cast<Instruction>(V))
      F = I->getFunction();
    if (!F)
      return nullptr;
    IRBuilder<> E(&*F->getEntryBlock().getFirstInsertionPt());
    AllocaInst* A = E.CreateAlloca(V->getType(), nullptr, "cd.vt");
    B.CreateStore(V, A);
    return B.CreateBitCast(A, i8p, "cd.vtp");
  }
  Value* resPtr(Instruction* I, IRBuilder<>& B) {
    LLVMContext& C = B.getContext();
    Type* i8p = PointerType::getUnqual(Type::getInt8Ty(C));
    if (I->getType()->isVoidTy() || isa<PHINode>(I))
      return nullptr;
    Function* F = I->getFunction();
    if (!F)
      return nullptr;
    IRBuilder<> E(&*F->getEntryBlock().getFirstInsertionPt());
    AllocaInst* A = E.CreateAlloca(I->getType(), nullptr, "cd.rt");
    B.CreateStore(I, A);
    return B.CreateBitCast(A, i8p, "cd.rtp");
  }
};
} // namespace
char InjectPass::ID = 0;
static RegisterPass<InjectPass> Z("inject", "CO-DIFT Inject", false, false);
