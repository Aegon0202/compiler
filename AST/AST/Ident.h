#pragma once
#include <string>

#include "ASTUnit.h"

namespace saltyfish {
	class ASTVisitor;

	class Ident
		:virtual public ASTUnit
	{
	public:
		std::string identStr;

	public:
		Ident();
		Ident(std::string identStr);
		Ident(std::string identStr, location loc);
		virtual void accept(ASTVisitor& visitor);
		friend std::ostream& operator<<(std::ostream& o, const Ident& ident);
	};
}