#include "BlockStmt.h"
#include "../ASTVisitor/ASTVisitor.h"

using namespace std;
using namespace saltyfish;

BlockStmt::BlockStmt() {}

BlockStmt::BlockStmt(std::vector<std::unique_ptr<ASTUnit>> stmts)
	:stmts(std::move(stmts))
{
	
}

BlockStmt::BlockStmt(std::vector<std::unique_ptr<ASTUnit>> stmts,location loc)
	:ASTUnit(loc), stmts(std::move(stmts))
{

}

void BlockStmt::accept(ASTVisitor& visitor) {
	visitor.visit(this);
}