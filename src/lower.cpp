#include "lower.h"

#include "ir.h"
#include "ir_mutator.h"
#include "usedef.h"
#include "sig.h"
#include "indexvar.h"
#include "util.h"

using namespace std;

namespace simit {
namespace ir {


class LoopVars : public SIGVisitor {
public:
  LoopVars() {}
  LoopVars(const SIG &sig) {apply(sig);}

  const Var &getVar(const IndexVar &var) const {
    return vertexLoopvars.at(var);
  }

private:
  map<IndexVar, Var> vertexLoopvars;
  map<Expr, Var> edgeLoopvars;

  void visit(const SIGVertex *v) {
    vertexLoopvars[v->iv] = Var(v->iv.getName(), Int(32));
  }
};


/// Specialize a statement containing an index expression to compute one value.
class SpecializeIndexExprStmt : public IRMutator {
public:
  SpecializeIndexExprStmt(const LoopVars &lvs) : lvs(lvs) {}

private:
  const LoopVars &lvs;

  void visit(const AssignStmt *op) {
    assert(isa<IndexExpr>(op->value) && "Can only specialize IndexExpr stmts");
    const IndexExpr *indexExpr = to<IndexExpr>(op->value);

    Var var = op->var;
    Expr value = mutate(indexExpr);

    if (indexExpr->resultVars.size() == 0) {
      stmt = AssignStmt::make(var, value);
    }
    else {
      Expr varExpr = VarExpr::make(var);
      std::vector<Expr> indices;
      for (IndexVar const& iv : indexExpr->resultVars) {
        indices.push_back(lvs.getVar(iv));
      }
      stmt = TensorWrite::make(varExpr, indices, value);
    }
  }

  void visit(const IndexedTensor *op) {
    if (isa<VarExpr>(op->tensor)) {
      if (op->indexVars.size() == 0) {
        expr = op->tensor;
      }
      else {
        std::vector<Expr> indices;
        for (IndexVar const& iv : op->indexVars) {
          indices.push_back(lvs.getVar(iv));
        }
        expr = TensorRead::make(op->tensor, indices);
      }
    }
    else {
      op->tensor.accept(this);
      // Flatten index expression. E.g. ((i) A(i,j) *  ((m) c(m)+b(m))(j)  )
      expr = op;
      NOT_SUPPORTED_YET;
    }
  }

  void visit(const IndexExpr *op) {
    expr = mutate(op->value);
  }
};


class GetReductionTmpName : public IRVisitor {
public:
  string get(const TensorWrite *op) {
    op->tensor.accept(this);
    for (auto &index : op->indices) {
      index.accept(this);
    }
    return name;
  }

private:
  std::string name;

  void visit(const VarExpr *op) {
    IRVisitor::visit(op);
    name += op->var.name;
  }
};

class ReduceOverVar : public IRMutator {
public:
  ReduceOverVar(Stmt rstmt, Var rvar, ReductionOperator rop)
      : rvar(rvar), rstmt(rstmt) {}

  Var getTmpVar() {return tmpVar;}

private:
  Var rvar;
  Stmt rstmt;
  ReductionOperator rop;

  Var tmpVar;

  void visit(const AssignStmt *op) {
    if (op == rstmt) {
      switch (rop.getKind()) {
        case ReductionOperator::Sum:
          Expr varExpr = VarExpr::make(op->var);
          stmt = AssignStmt::make(op->var, Add::make(varExpr, op->value));
          break;
      }
    }
    else {
      stmt = op;
    }
  }

  void visit(const TensorWrite *op) {
    if (op == rstmt) {
      Expr tensor = op->tensor;
      std::vector<Expr> indices = op->indices;

      assert(tensor.type().isTensor());
      switch (rop.getKind()) {
        case ReductionOperator::Sum:
          string tmpVarName = GetReductionTmpName().get(op);
          ScalarType componentType = tensor.type().toTensor()->componentType;
          tmpVar = Var(tmpVarName, TensorType::make(componentType));
          stmt = AssignStmt::make(tmpVar, Add::make(tmpVar, op->value));
          break;
      }
    }
  }
};


class LoopBuilder : public SIGVisitor {
public:
  LoopBuilder(const UseDef *ud) : ud(ud) {}

  Stmt create(const IndexExpr *indexExpr, Stmt indexStmt) {
    SIG sig = SIGBuilder(ud).create(indexExpr);
    lvs = LoopVars(sig);

    componentType = indexExpr->type.toTensor()->componentType;
    body = SpecializeIndexExprStmt(lvs).mutate(indexStmt);
    stmt = body;

    apply(sig);
    Stmt result = stmt;
    stmt = Stmt();
    return result;
  }

private:
  const UseDef *ud;
  LoopVars lvs;
  ScalarType componentType;
  Stmt body;

  Stmt stmt;

  void visit(const SIGVertex *v) {
    SIGVisitor::visit(v);

    const SIGEdge *previous = getPreviousEdge();
    if (previous == nullptr) {
      // The vertex is unconstrained and loops over it's whole domain.
      const IndexSet &domain = v->iv.getDomain().getIndexSets()[0];

      if (v->iv.isFreeVar()) {
        stmt = For::make(lvs.getVar(v->iv), domain, stmt);
      }
      else {
        ReduceOverVar rov(body, lvs.getVar(v->iv), v->iv.getOperator());
        Stmt loopBody = rov.mutate(stmt);
        Var tmpVar = rov.getTmpVar();
        Stmt alloc = AssignStmt::make(tmpVar, Literal::make(tmpVar.type, {0}));
        Stmt loop = For::make(lvs.getVar(v->iv), domain, loopBody);
        stmt = Block::make(alloc, loop);
      }
    }
    else {
      // The vertex is constrained and loops over previous' endpoints.
      const IndexSet &domain = v->iv.getDomain().getIndexSets()[0];
      stmt = For::make(Var("derived", Int(32)), domain, stmt);
      NOT_SUPPORTED_YET;
    }
  }

  void visit(const SIGEdge *e) {
    SIGVisitor::visit(e);
    // Emit loops
  }
};


class LowerIndexExpressions : public IRMutator {
public:
  LowerIndexExpressions(const UseDef *ud) : ud(ud) {}

private:
  const UseDef *ud;

  Stmt lower(const IndexExpr *indexExpr, Stmt stmt) {
    return LoopBuilder(ud).create(indexExpr, stmt);
  }

  void visit(const IndexExpr *op) {
    assert(false &&
           "IndexExprs must be assigned to a var/field/tensor before lowering");
  }

  void visit(const AssignStmt *op) {
    if (isa<IndexExpr>(op->value)) {
      stmt = lower(to<IndexExpr>(op->value), op);
    }
    else {
      IRMutator::visit(op);
    }
  }

  void visit(const FieldWrite *op) {
    NOT_SUPPORTED_YET;
  }

  void visit(const TensorWrite *op) {
    NOT_SUPPORTED_YET;
  }
};

Func lowerIndexExpressions(Func func) {
  UseDef ud(func);
//  cout << ud << endl;
  return LowerIndexExpressions(&ud).mutate(func);
}
}}
