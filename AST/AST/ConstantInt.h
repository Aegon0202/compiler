#pragma once
#include <map>
#include <iostream>
#include "Constant.h"

namespace saltyfish {
	class ASTVisitor;

	class ConstantInt:public Constant {
	public:
		enum ConstantIntType { Octal, Decimal, Hexadecimal } constantIntType;
		int value;
	public:
		static std::map<ConstantIntType, std::string> constantIntTypeMap;
	public:
		ConstantInt();
		ConstantInt(ConstantInt::ConstantIntType constantIntType, int value);
		ConstantInt(ConstantInt::ConstantIntType constantIntType, int value,saltyfish::location loc);
		virtual void accept(ASTVisitor& visitor);
		friend std::ostream& operator<<(std::ostream& o, const ConstantInt& constantInt);
	};
}