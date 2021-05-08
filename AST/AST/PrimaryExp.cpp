#include "Exp.h"
#include "PrimaryExp.h"
#include "../ASTVisitor/ASTVisitor.h"

using namespace saltyfish;
using namespace std;

std::map<PrimaryExp::PrimaryExpType, std::string> PrimaryExp::primaryExpTypeMap = {
	{PrimaryExp::PrimaryExpType::Ident,		 "ident"},
	{PrimaryExp::PrimaryExpType::ConstantInt,"constantInt"}
};

PrimaryExp::PrimaryExp(PrimaryExpType primaryExpType, std::unique_ptr<saltyfish::Ident> ident)
	:primaryExpType(primaryExpType),ident(std::move(ident))
{

}

PrimaryExp::PrimaryExp(PrimaryExpType primaryExpType, std::unique_ptr<saltyfish::Constant> constant)
	: primaryExpType(primaryExpType), constant(std::move(constant))
{

}

PrimaryExp::PrimaryExp(PrimaryExpType primaryExpType, std::unique_ptr<saltyfish::Ident> ident,location loc)
	:ASTUnit(loc), primaryExpType(primaryExpType), ident(std::move(ident))
{

}

PrimaryExp::PrimaryExp(PrimaryExpType primaryExpType, std::unique_ptr<saltyfish::Constant> constant,location loc)
	:ASTUnit(loc), primaryExpType(primaryExpType), constant(std::move(constant))
{

}

PrimaryExp::~PrimaryExp()
{
	
}

void PrimaryExp::accept(ASTVisitor& visitor) {
	visitor.visit(this);
}

namespace saltyfish {
	std::ostream& operator<<(std::ostream& o, const PrimaryExp& primaryExp)
	{
		o << "<";
		o << "PrimaryExp " << "\'" << PrimaryExp::primaryExpTypeMap.at(primaryExp.primaryExpType) << "\' ";
		cout << "\'" << primaryExp.loc << "\'";
		o << ">";
		return o;
	}
}