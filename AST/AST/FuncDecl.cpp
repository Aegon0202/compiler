#include "FuncDecl.h"
#include "../ASTVisitor/ASTVisitor.h"

using namespace std;
using namespace saltyfish;

FuncDecl::FuncDecl(unique_ptr<Ident> ident)
	:ident(std::move(ident))
{

}

FuncDecl::FuncDecl(unique_ptr<Ident> ident,location loc)
	:ASTUnit(loc), ident(std::move(ident))
{

}

void FuncDecl::accept(ASTVisitor& visitor) {
	visitor.visit(this);
}