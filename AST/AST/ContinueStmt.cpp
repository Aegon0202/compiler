#include "ContinueStmt.h"
#include "../ASTVisitor/ASTVisitor.h"

using namespace saltyfish;

ContinueStmt::ContinueStmt() {}

ContinueStmt::ContinueStmt(location loc):ASTUnit(loc) {}

void ContinueStmt::accept(ASTVisitor& visitor) {
	visitor.visit(this);
}