#include "ExpStmt.h"
#include "../ASTVisitor/ASTVisitor.h"

using namespace std;
using namespace saltyfish;

ExpStmt::ExpStmt(std::unique_ptr<Exp> exp)
	:exp(std::move(exp))
{
	
}

ExpStmt::ExpStmt(std::unique_ptr<Exp> exp,location loc)
	:ASTUnit(loc), exp(std::move(exp))
{

}


void ExpStmt::accept(ASTVisitor &visitor) {
	visitor.visit(this);
}
