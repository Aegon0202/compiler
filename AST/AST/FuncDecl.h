#pragma once
#include <memory>

#include "Ident.h"
#include "Decl.h"

namespace saltyfish {
	class ASTVisitor;

	class FuncDecl:public Decl
	{
	public:
		std::unique_ptr<Ident> ident;

	public:
		FuncDecl(std::unique_ptr<Ident> ident);
		FuncDecl(std::unique_ptr<Ident> ident,location loc);
		virtual void accept(ASTVisitor& visitor);
	};
}

