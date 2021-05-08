#include "IfStmt.h"
#include "../ASTVisitor/ASTVisitor.h"

using namespace std;
using namespace saltyfish;

IfStmt::IfStmt(std::unique_ptr<Exp> cond, std::unique_ptr<Stmt> ifBody)
	:cond(std::move(cond)), ifBody(std::move(ifBody))
{
	setHasElse(false);
}

IfStmt::IfStmt(std::unique_ptr<Exp> cond, std::unique_ptr<Stmt> ifBody, std::unique_ptr<Stmt> elseBody)
	: cond(std::move(cond)), ifBody(std::move(ifBody)), elseBody(std::move(elseBody))
{
	setHasElse(true);
}

IfStmt::IfStmt(std::unique_ptr<Exp> cond, std::unique_ptr<Stmt> ifBody,location loc)
	:ASTUnit(loc), cond(std::move(cond)), ifBody(std::move(ifBody))
{
	setHasElse(false);
}

IfStmt::IfStmt(std::unique_ptr<Exp> cond, std::unique_ptr<Stmt> ifBody, std::unique_ptr<Stmt> elseBody,location loc)
	:ASTUnit(loc), cond(std::move(cond)), ifBody(std::move(ifBody)), elseBody(std::move(elseBody))
{
	setHasElse(true);
}

bool IfStmt::hasElse() {
	return bitFields.hasElse == 1 ? true : false;
}

void IfStmt::setHasElse(bool b) {
	bitFields.hasElse = b ? 1 : 0;
}

void IfStmt::accept(ASTVisitor& visitor) {
	visitor.visit(this);
}