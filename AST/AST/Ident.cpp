#include <string>

#include "Ident.h"
#include "../ASTVisitor/ASTVisitor.h"

using namespace saltyfish;
using namespace std;

Ident::Ident() {}
Ident::Ident(string identStr) :identStr(identStr) {}
Ident::Ident(string identStr,location loc) 
	:ASTUnit(loc),identStr(identStr) {}

void Ident::accept(ASTVisitor& visitor) {
	visitor.visit(this);
}

namespace saltyfish {
	std::ostream& operator<<(std::ostream& o, const Ident& ident)
	{
		o << "<";
		o << "Ident " << "\'" << ident.identStr << "\' ";
		o << "\'" << ident.loc << "\'";
		cout << ">";
		return o;
	}
}
