#include <vector>
#include <memory>

#include "CompUnit.h"
#include "../ASTVisitor/ASTVisitor.h"

using namespace std;
using namespace saltyfish;

CompUnit::CompUnit() {
	
}

CompUnit::CompUnit(std::vector<std::unique_ptr<ASTUnit>> compUnitList)
	:compUnitList(std::move(compUnitList))
{
	
}

CompUnit::CompUnit(std::vector<std::unique_ptr<ASTUnit>> compUnitList,location loc)
	:ASTUnit(loc), compUnitList(std::move(compUnitList))
{

}

void CompUnit::accept(ASTVisitor& visitor) {
	visitor.visit(this);
}