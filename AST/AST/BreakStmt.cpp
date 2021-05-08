#include "BreakStmt.h"
#include "../ASTVisitor/ASTVisitor.h"

using namespace std;
using namespace saltyfish;

BreakStmt::BreakStmt() {
	
}

BreakStmt::BreakStmt(location loc)
	:ASTUnit(loc)
{

}

void BreakStmt::accept(ASTVisitor& visitor) {
	visitor.visit(this);
}