#pragma once
#include <string>
#include <memory>
#include <map>

#include "Exp.h"

namespace saltyfish {
	class ASTVisitor;

	class BinaryExp:public Exp {
	public:
		enum BinaryExpType {
			Mul, Div, Mod, Add, Sub,
			B, A, Be, Ae, E, Ne,
			And, Or,
			Equal
		} binaryExpType;
		std::string opStr;
		std::unique_ptr<Exp> Lexp;
		std::unique_ptr<Exp> Rexp;
	public:
		static std::map<BinaryExpType, std::string> BinaryExpTypeMap;
	public:
		BinaryExp();
		BinaryExp(BinaryExpType binaryExpType, std::unique_ptr<Exp> Lexp, std::unique_ptr <Exp> Rexp);
		BinaryExp(BinaryExpType binaryExpType, std::unique_ptr<Exp> Lexp, std::unique_ptr <Exp> Rexp,location loc);
		virtual void accept(ASTVisitor& visitor);
		friend std::ostream& operator<<(std::ostream& o, const BinaryExp& binaryExp);
	};
}