#include "Type.h"
#include "../ASTVisitor/ASTVisitor.h"

namespace saltyfish {
	Type::Type() {
		
	}

	Type::Type(std::string &typeStr) :typeStr(typeStr) {
		
	}

	Type::Type(std::string& typeStr,location loc)
		:ASTUnit(loc), typeStr(typeStr) {

	}

	void Type::accept(ASTVisitor& visitor) {
		visitor.visit(this);
	}

	std::ostream& operator<<(std::ostream& o, const Type& type)
	{
		o << "<";
		o << "Type " << "\'" << type.typeStr << "\' ";
		cout << "\'" << type.loc << "\'";
		o << ">";
		return o;
	}
}