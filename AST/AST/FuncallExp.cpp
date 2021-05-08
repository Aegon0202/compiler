#include "FuncallExp.h"
#include "../ASTVisitor/ASTVisitor.h"

#include <string>
#include <vector>

#include "Ident.h"
#include "Exp.h"

using namespace std;
using namespace saltyfish;

FuncallExp::FuncallExp(std::unique_ptr<Ident> ident)
	:ident(std::move(ident))
{
	
}

FuncallExp::FuncallExp(std::unique_ptr<Ident> ident, std::vector<std::unique_ptr<Exp>> funcallParamList)
	:ident(std::move(ident)),funcallParamList(std::move(funcallParamList)) {
	
}

FuncallExp::FuncallExp(std::unique_ptr<Ident> ident,location loc)
	:ASTUnit(loc), ident(std::move(ident))
{

}

FuncallExp::FuncallExp(std::unique_ptr<Ident> ident, std::vector<std::unique_ptr<Exp>> funcallParamList,location loc)
	:ASTUnit(loc), ident(std::move(ident)), funcallParamList(std::move(funcallParamList)) {

}

void FuncallExp::accept(ASTVisitor& visitor) {
	visitor.visit(this);
}