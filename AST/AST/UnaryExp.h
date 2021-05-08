#pragma once
#include <string>
#include <memory>
#include <map>

#include "Exp.h"

namespace saltyfish {
	class ASTVisitor;

	class UnaryExp:public Exp
	{
	public:
		enum UnaryExpType {
			Add,Sub,Not
		} unaryExpType;
		std::string opStr;
		std::unique_ptr<Exp> exp;
	public:
		static std::map<UnaryExpType, std::string> unaryExpTypeMap;
	public:
		UnaryExp(UnaryExpType unaryExpType,std::unique_ptr<Exp> exp);
		UnaryExp(UnaryExpType unaryExpType, std::unique_ptr<Exp> exp,location loc);
		virtual void accept(ASTVisitor& visitor);
		friend std::ostream& operator<<(std::ostream& o, const UnaryExp& unaryExp);
	};

}

