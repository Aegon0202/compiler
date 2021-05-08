#include "EmptyStmt.h"
#include "../ASTVisitor/ASTVisitor.h"

using namespace saltyfish;

EmptyStmt::EmptyStmt()
{

}

EmptyStmt::EmptyStmt(location loc)
	:ASTUnit(loc)
{

}

void EmptyStmt::accept(ASTVisitor& visitor) {
	visitor.visit(this);
}