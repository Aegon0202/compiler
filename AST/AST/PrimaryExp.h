#pragma once
#include <memory>
#include <map>

#include "Exp.h"
#include "Ident.h"
#include "Constant.h"

namespace saltyfish {
	class ASTVisitor;

	class PrimaryExp:public Exp
	{
	public:
		enum PrimaryExpType {
			Ident, ConstantInt
		} primaryExpType;
		union {
			std::unique_ptr<saltyfish::Ident> ident;
			std::unique_ptr<saltyfish::Constant> constant;
		};
	public:
		static std::map<PrimaryExpType, std::string> primaryExpTypeMap;
	public:
		PrimaryExp(PrimaryExpType primaryExpType, std::unique_ptr<saltyfish::Ident> ident);
		PrimaryExp(PrimaryExpType primaryExpType, std::unique_ptr<saltyfish::Constant> constant);
		PrimaryExp(PrimaryExpType primaryExpType, std::unique_ptr<saltyfish::Ident> ident,location loc);
		PrimaryExp(PrimaryExpType primaryExpType, std::unique_ptr<saltyfish::Constant> constant,location loc);
		~PrimaryExp();
		virtual void accept(ASTVisitor& visitor);
		friend std::ostream& operator<<(std::ostream& o, const PrimaryExp& primaryExp);
	};
}

