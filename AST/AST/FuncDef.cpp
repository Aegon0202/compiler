#include "FuncDef.h"
#include "../ASTVisitor/ASTVisitor.h"

using namespace std;
using namespace saltyfish;

FuncDef::FuncDef(std::unique_ptr<Type> type, std::unique_ptr<Ident> ident, std::unique_ptr<BlockStmt> block)
	:type(std::move(type)),ident(std::move(ident)),block(std::move(block))
{
	
}

FuncDef::FuncDef(std::unique_ptr<Type> type, std::unique_ptr<Ident> ident, std::vector<std::unique_ptr<FuncParamDecl>> funcParamDeclList, std::unique_ptr<BlockStmt> block)
	: type(std::move(type)), ident(std::move(ident)),funcParamDeclList(std::move(funcParamDeclList)), block(std::move(block))
{
	
}

FuncDef::FuncDef(std::unique_ptr<Type> type, std::unique_ptr<Ident> ident, std::unique_ptr<BlockStmt> block,location loc)
	:ASTUnit(loc), type(std::move(type)), ident(std::move(ident)), block(std::move(block))
{

}

FuncDef::FuncDef(std::unique_ptr<Type> type, std::unique_ptr<Ident> ident, std::vector<std::unique_ptr<FuncParamDecl>> funcParamDeclList, std::unique_ptr<BlockStmt> block,location loc)
	:ASTUnit(loc), type(std::move(type)), ident(std::move(ident)), funcParamDeclList(std::move(funcParamDeclList)), block(std::move(block))
{

}

void FuncDef::accept(ASTVisitor& visitor) {
	visitor.visit(this);
}