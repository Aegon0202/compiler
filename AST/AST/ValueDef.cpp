#include <string>
#include <memory>
#include <iostream>
#include <utility>
#include "ValueDef.h"
#include "ConstantInt.h"
#include "../ASTVisitor/ASTVisitor.h"

using namespace saltyfish;

ValueDef::ValueDef() {}

ValueDef::ValueDef(unique_ptr<Ident> ident, vector<unique_ptr<Exp>> arrayDimList)
	:ident(std::move(ident)), arrayDimList(std::move(arrayDimList))
{
	setHasAssign(false);
	setIsArray(!this->arrayDimList.empty());
	if (isArray()) {
		setIsArrayFirstDimEmpty((this->arrayDimList)[0] == nullptr);
	}
}

ValueDef::ValueDef(unique_ptr<Ident> ident, vector<unique_ptr<Exp>> arrayDimList, unique_ptr<Exp> exp)
	: ident(std::move(ident)), arrayDimList(std::move(arrayDimList)), exp(std::move(exp))
{
	setHasAssign(true);
	setIsArray(!this->arrayDimList.empty());
	if (isArray()) {
		setIsArrayFirstDimEmpty((this->arrayDimList)[0] == nullptr);
	}
}

ValueDef::ValueDef(unique_ptr<Ident> ident, vector<unique_ptr<Exp>> arrayDimList,location loc)
	:ASTUnit(loc), ident(std::move(ident)), arrayDimList(std::move(arrayDimList))
{
	setHasAssign(false);
	setIsArray(!this->arrayDimList.empty());
	if (isArray()) {
		setIsArrayFirstDimEmpty((this->arrayDimList)[0] == nullptr);
	}
}

ValueDef::ValueDef(unique_ptr<Ident> ident, vector<unique_ptr<Exp>> arrayDimList, unique_ptr<Exp> exp,location loc)
	:ASTUnit(loc), ident(std::move(ident)), arrayDimList(std::move(arrayDimList)), exp(std::move(exp))
{
	setHasAssign(true);
	setIsArray(!this->arrayDimList.empty());
	if (isArray()) {
		setIsArrayFirstDimEmpty((this->arrayDimList)[0] == nullptr);
	}
}

bool ValueDef::hasAssign() {
	return bitFields.hasAssign == 1 ? true : false;
}

bool ValueDef::isArrayAssign() {
	return bitFields.isArrayAssign == 1 ? true : false;
}

bool ValueDef::isArray() {
	return bitFields.isArray == 1 ? true : false;
}

bool ValueDef::isArrayFirstDimEmpty() {
	return bitFields.isArrayFirstDimEmpty == 1 ? true : false;
}

void ValueDef::setHasAssign(bool b) {
	bitFields.hasAssign = b ? 1 : 0;
}

void ValueDef::setIsArrayAssign(bool b) {
	bitFields.isArrayAssign = b ? 1 : 0;
}

void ValueDef::setIsArray(bool b) {
	bitFields.isArray = b ? 1 : 0;
}

void ValueDef::setIsArrayFirstDimEmpty(bool b) {
	bitFields.isArrayFirstDimEmpty = b ? 1 : 0;
}

void ValueDef::accept(ASTVisitor& visitor) {
	visitor.visit(this);
}