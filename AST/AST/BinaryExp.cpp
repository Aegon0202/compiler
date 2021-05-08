#include "BinaryExp.h"
#include "../ASTVisitor/ASTVisitor.h"

using namespace saltyfish;
using namespace std;

std::map<BinaryExp::BinaryExpType, std::string> BinaryExp::BinaryExpTypeMap = {
	{BinaryExp::BinaryExpType::Mul,"Mul *"},
	{BinaryExp::BinaryExpType::Div,"Div /"},
	{BinaryExp::BinaryExpType::Mod,"Mod %"},
	{BinaryExp::BinaryExpType::Add,"Add +"},
	{BinaryExp::BinaryExpType::Sub,"Sub -"},
	{BinaryExp::BinaryExpType::B,  "B   <"},
	{BinaryExp::BinaryExpType::A,  "A   >"},
	{BinaryExp::BinaryExpType::Be, "Be <="},
	{BinaryExp::BinaryExpType::Ae, "Ae >="},
	{BinaryExp::BinaryExpType::E,  "E  =="},
	{BinaryExp::BinaryExpType::Ne, "Ne !="},
	{BinaryExp::BinaryExpType::And,"And&&"},
	{BinaryExp::BinaryExpType::Or, "Or ||"},
	{BinaryExp::BinaryExpType::Equal,"Equal=="}
};

BinaryExp::BinaryExp() {}

BinaryExp::BinaryExp(BinaryExp::BinaryExpType binaryExpType, unique_ptr<Exp> Lexp, unique_ptr<Exp> Rexp)
	:binaryExpType(binaryExpType), Lexp(std::move(Lexp)), Rexp(std::move(Rexp))
{
	
}

BinaryExp::BinaryExp(BinaryExp::BinaryExpType binaryExpType, unique_ptr<Exp> Lexp, unique_ptr<Exp> Rexp,location loc)
	:ASTUnit(loc), binaryExpType(binaryExpType), Lexp(std::move(Lexp)), Rexp(std::move(Rexp))
{

}

void BinaryExp::accept(ASTVisitor& visitor) {
	visitor.visit(this);
}

namespace saltyfish {
	std::ostream& operator<<(std::ostream& o, const BinaryExp& binaryExp)
	{
		o << "<";
		o << "BinaryExp \'" << BinaryExp::BinaryExpTypeMap.at(binaryExp.binaryExpType) << "\' ";
		cout << "\'" << binaryExp.loc << "\'";
		o << ">";
		return o;
	}
}
