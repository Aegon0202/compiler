#include <memory>
#include <vector>

#include "WhileStmt.h"
#include "../ASTVisitor/ASTVisitor.h"

using namespace std;
using namespace saltyfish;

WhileStmt::WhileStmt(std::unique_ptr<Exp> cond, std::unique_ptr<Stmt> body)
	:cond(std::move(cond)), body(std::move(body))
{
	
}

WhileStmt::WhileStmt(std::unique_ptr<Exp> cond, std::unique_ptr<Stmt> body,location loc)
	:ASTUnit(loc), cond(std::move(cond)), body(std::move(body))
{

}

void WhileStmt::accept(ASTVisitor& visitor) {
	visitor.visit(this);
}