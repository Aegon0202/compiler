#pragma once
#include "../../location.hh"

namespace saltyfish {
	class ASTVisitor;

	class ASTUnit
	{
	public:
		location loc;
	public:
		ASTUnit();
		ASTUnit(location loc);
		virtual void accept(ASTVisitor& visitor);
	};
}
