#include "ASTUnit.h"

using namespace saltyfish;

ASTUnit::ASTUnit() {
	
}

ASTUnit::ASTUnit(location loc)
	:loc(loc)
{
	
}

void ASTUnit::accept(ASTVisitor& visitor) {
	
}