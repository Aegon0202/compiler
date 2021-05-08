#include "ReturnStmt.h"
#include "Exp.h"
#include "../ASTVisitor/ASTVisitor.h"

using namespace std;
using namespace saltyfish;

ReturnStmt::ReturnStmt()
{
	setHasExp(false);
}

ReturnStmt::ReturnStmt(std::unique_ptr<Exp> exp)
	:exp(std::move(exp))
{
	setHasExp(true);
}

ReturnStmt::ReturnStmt(location loc)
	:ASTUnit(loc)
{
	setHasExp(false);
}

ReturnStmt::ReturnStmt(std::unique_ptr<Exp> exp,location loc)
	:ASTUnit(loc),exp(std::move(exp))
{
	setHasExp(true);
}

bool ReturnStmt::hasExp() {
	return bitFields.hasExp == 1 ? true : false;
}

void ReturnStmt::setHasExp(bool b)
{
	bitFields.hasExp = b ? 1 : 0;
}

void ReturnStmt::accept(ASTVisitor& visitor) {
	visitor.visit(this);
}