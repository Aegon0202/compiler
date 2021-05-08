#include <iostream>
#include <vector>
#include <memory>

#include "MessageVisitor.h"

using namespace std;
using namespace saltyfish;

MessageVisitor::MessageVisitor() {}
MessageVisitor::~MessageVisitor() {}

void MessageVisitor::visit() {
	showDepth();
	cout << "Nothing" << endl;
}

void MessageVisitor::visit(CompUnit* compUnit) {
	showDepth();
	cout << "CompUnit" << endl;
	depth++;
	vector<unique_ptr<ASTUnit>>& compUnitList = compUnit->compUnitList;
	for (vector<unique_ptr<ASTUnit>>::iterator it = compUnitList.begin(); it < compUnitList.end(); it++) {
		(*it)->accept(*this);
	}
	depth--;
}

void MessageVisitor::visit(FuncDecl* funcDecl){
	showDepth();
	cout << "FuncDecl" << endl;
}

void MessageVisitor::visit(FuncParamDecl* funcParamDecl) {
	showDepth();
	cout << *funcParamDecl << endl;
	depth++;
	funcParamDecl->type->accept(*this);
	funcParamDecl->ident->accept(*this);
	depth--;
}

void MessageVisitor::visit(ValueDecl* valueDecl){
	showDepth();
	cout << "ValueDecl" << endl;
	depth++;
	unique_ptr<Type>& type = valueDecl->type;
	vector<unique_ptr<ValueDef>>& valueDefList = valueDecl->ValueDefList;
	for (vector<unique_ptr<ValueDef>>::iterator it = valueDefList.begin(); it < valueDefList.end(); it++) {
		(*it)->accept(*this);
	}
	depth--;
}

void MessageVisitor::visit(ValueDef* valueDef) {
	showDepth();
	cout << "ValueDef" << endl;
	depth++;
	valueDef->ident->accept(*this);
	if (valueDef->isArray()) {
		vector<unique_ptr<Exp>>& arrayDimList = valueDef->arrayDimList;
		vector<unique_ptr<Exp>>::iterator it = arrayDimList.begin();
		if (valueDef->isArrayFirstDimEmpty()) {
			it++;
		}
		for (it; it < arrayDimList.end(); it++) {
			(*it)->accept(*this);
		}
	}
	if (valueDef->hasAssign()) {
		valueDef->exp->accept(*this);
	}
	depth--;
}

void MessageVisitor::visit(FuncDef* funcDef) {
	showDepth();
	cout << "FuncDef" << endl;
	depth++;
	vector<unique_ptr<FuncParamDecl>>& funcParamDeclList = funcDef->funcParamDeclList;
	unique_ptr<BlockStmt>& block = funcDef->block;
	funcDef->type->accept(*this);
	funcDef->ident->accept(*this);
	for (vector<unique_ptr<FuncParamDecl>>::iterator it = funcParamDeclList.begin(); it < funcParamDeclList.end(); it++) {
		(*it)->accept(*this);
	}
	block->accept(*this);
	depth--;
}

void MessageVisitor::visit(AssignStmt* assignStmt){
	showDepth();
	cout << "AssignStmt" << endl;
	depth++;
	unique_ptr<Ident>& ident = assignStmt->ident;
	unique_ptr<Exp>& exp = assignStmt->exp;
	ident->accept(*this);
	exp->accept(*this);
	depth--;
}
void MessageVisitor::visit(BlockStmt* blockStmt){
	showDepth();
	cout << "BlockStmt" << endl;
	depth++;
	vector<unique_ptr<ASTUnit>>& stmts = blockStmt->stmts;
	for (vector<unique_ptr<ASTUnit>>::iterator it = stmts.begin(); it < stmts.end(); it++) {
		(*it)->accept(*this);
	}
	depth--;
}
void MessageVisitor::visit(BreakStmt* breakStmt){
	showDepth();
	cout << "BreakStmt" << endl;
}
void MessageVisitor::visit(ContinueStmt* continueStmt){
	showDepth();
	cout << "ContinueStmt" << endl;
}
void MessageVisitor::visit(EmptyStmt* emptyStmt){
	showDepth();
	cout << "EmptyStmt" << endl;
}
void MessageVisitor::visit(ExpStmt* expStmt){
	showDepth();
	cout << "ExpStmt" << endl;
	depth++;
	unique_ptr<Exp>& exp = expStmt->exp;
	exp->accept(*this);
	depth--;
}
void MessageVisitor::visit(IfStmt* ifStmt){
	showDepth();
	cout << "IfStmt" << endl;
	depth++;
	unique_ptr<Exp> &cond = ifStmt->cond;
	unique_ptr<Stmt>& ifBody = ifStmt->ifBody;
	cond->accept(*this);
	ifBody->accept(*this);
	if (ifStmt->hasElse()) {
		unique_ptr<Stmt>& elseBody = ifStmt->elseBody;
		elseBody->accept(*this);
	}
	depth--;
}
void MessageVisitor::visit(ReturnStmt* returnStmt){
	showDepth();
	cout << "ReturnStmt" << endl;
	depth++;
	if (returnStmt->hasExp()) {
		unique_ptr<Exp>& exp = returnStmt->exp;
		exp->accept(*this);
	}
	depth--;
}
void MessageVisitor::visit(WhileStmt* whileStmt){
	showDepth();
	cout << "WhileStmt" << endl;
	depth++;
	unique_ptr<Exp> &cond = whileStmt->cond;
	unique_ptr<Stmt>& body = whileStmt->body;
	cond->accept(*this);
	body->accept(*this);
	depth--;
}

void MessageVisitor::visit(BinaryExp* binaryExp){
	showDepth();
	cout << *binaryExp << endl;
	depth++;
	binaryExp->Lexp->accept(*this);
	binaryExp->Rexp->accept(*this);
	depth--;
}
void MessageVisitor::visit(FuncallExp* funcallExp){
	showDepth();
	cout << "FuncallExp" << endl;
	depth++;
	vector<unique_ptr<Exp>>& funcParamList = funcallExp->funcallParamList;
	funcallExp->ident->accept(*this);
	for (vector<unique_ptr<Exp>>::iterator it = funcParamList.begin(); it < funcParamList.end(); it++) {
		(*it)->accept(*this);
	}
	depth--;
}
void MessageVisitor::visit(PrimaryExp* primaryExp){
	showDepth();
	cout << *primaryExp << endl;
	depth++;
	if (primaryExp->primaryExpType == PrimaryExp::PrimaryExpType::Ident) {
		primaryExp->ident->accept(*this);
	}
	else {
		primaryExp->constant->accept(*this);
	}
	depth--;
}
void MessageVisitor::visit(UnaryExp* unaryExp){
	showDepth();
	cout << *unaryExp << endl;
	unaryExp->exp->accept(*this);
	depth--;
}

void MessageVisitor::visit(Type* type){
	showDepth();
	cout << *type << endl;
}
void MessageVisitor::visit(ConstantInt* constantInt){
	depth++;
	showDepth();
	cout << *constantInt << endl;
	depth--;
}
void MessageVisitor::visit(Ident* ident){
	showDepth();
	cout << *ident << endl;
}

void MessageVisitor::showDepth() {
	for (int i = 0; i < depth; i++) {
		cout << " |";
	}
}