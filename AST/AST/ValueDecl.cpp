#include "ValueDecl.h"
#include "../ASTVisitor/ASTVisitor.h"

using namespace std;
using namespace saltyfish;

ValueDecl::ValueDecl(unique_ptr<Type> type, std::vector<unique_ptr<ValueDef>> ValueDefList)
	:type(std::move(type)),ValueDefList(std::move(ValueDefList))
{
	
}

ValueDecl::ValueDecl(unique_ptr<Type> type, std::vector<unique_ptr<ValueDef>> ValueDefList,location loc)
	:ASTUnit(loc), type(std::move(type)), ValueDefList(std::move(ValueDefList))
{

}

void ValueDecl::accept(ASTVisitor& visitor) {
	visitor.visit(this);
}
