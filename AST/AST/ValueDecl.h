#pragma once
#include <vector>
#include <memory>

#include "Type.h"
#include "Decl.h"
#include "ValueDef.h"

namespace saltyfish {
	class ASTVisitor;

	class ValueDecl:public Decl
	{
	public:
		unique_ptr<Type> type;
		std::vector<unique_ptr<ValueDef>> ValueDefList;
	public:
		ValueDecl(unique_ptr<Type> type,std::vector<unique_ptr<ValueDef>> ValueDefList);
		ValueDecl(unique_ptr<Type> type, std::vector<unique_ptr<ValueDef>> ValueDefList,location loc);
		virtual void accept(ASTVisitor& visitor);
	};
}
