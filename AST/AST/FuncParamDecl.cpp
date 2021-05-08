#include "FuncParamDecl.h"
#include "Type.h"
#include "../ASTVisitor/ASTVisitor.h"

using namespace std;
using namespace saltyfish;

FuncParamDecl::FuncParamDecl() {}

FuncParamDecl::FuncParamDecl(unique_ptr<Type> type, unique_ptr<Ident> ident)
	:type(std::move(type)), ident(std::move(ident))
{

}

FuncParamDecl::FuncParamDecl(unique_ptr<Type> type, unique_ptr<Ident> ident,location loc)
	:ASTUnit(loc), type(std::move(type)), ident(std::move(ident))
{

}

void FuncParamDecl::accept(ASTVisitor& visitor) {
	visitor.visit(this);
}

namespace saltyfish {
	std::ostream& operator<<(std::ostream& o, const FuncParamDecl& funcParamDecl)
	{
		o << "<";
		o << "FuncParamDecl";
		o << ">";
		return o;
	}
}
