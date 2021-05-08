#pragma once
#include <memory>

#include "Ident.h"
#include "Type.h"
#include "Decl.h"

namespace saltyfish {
	class ASTVisitor;

    class FuncParamDecl :
        public Decl
    {
	public:
		std::unique_ptr<Type> type;
		std::unique_ptr<Ident> ident;

	public:
		FuncParamDecl();
		FuncParamDecl(std::unique_ptr<Type> type,std::unique_ptr<Ident> ident);
		FuncParamDecl(std::unique_ptr<Type> type, std::unique_ptr<Ident> ident,location loc);
		virtual void accept(ASTVisitor& visitor);
		friend std::ostream& operator<<(std::ostream& o, const FuncParamDecl& funcParamDecl);
	};
}

