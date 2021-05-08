#pragma once
#include "Type.h"
#include "ASTUnit.h"

namespace saltyfish {
	class ASTVisitor;

	class Decl :
		virtual public ASTUnit
	{
	public:
		virtual void accept(ASTVisitor& visitor) {
			
		}
	};
}

