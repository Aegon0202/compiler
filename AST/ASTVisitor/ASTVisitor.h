#pragma once
#include "CompUnit.h"
#include "Ident.h"
#include "Type.h"
#include "ConstantInt.h"
#include "EmptyStmt.h"
#include "ExpStmt.h"
#include "AssignStmt.h"
#include "BlockStmt.h"
#include "BreakStmt.h"
#include "ContinueStmt.h"
#include "IfStmt.h"
#include "ReturnStmt.h"
#include "WhileStmt.h"
#include "FuncDecl.h"
#include "ValueDecl.h"
#include "FuncParamDecl.h"
#include "ValueDef.h"
#include "FuncDef.h"
#include "BinaryExp.h"
#include "FuncallExp.h"
#include "PrimaryExp.h"
#include "UnaryExp.h"

namespace saltyfish {
	class ASTVisitor
	{
	public:
		virtual void visit() = 0;

		virtual void visit(CompUnit* compUnit) = 0;

		virtual void visit(FuncDecl* funcDecl) = 0;
		virtual void visit(FuncParamDecl* funcParamDecl) = 0;
		virtual void visit(ValueDecl* valueDecl) = 0;

		virtual void visit(ValueDef* valueDef) = 0;
		virtual void visit(FuncDef* funcDef) = 0;

		virtual void visit(AssignStmt* assignStmt) = 0;
		virtual void visit(BlockStmt* blockStmt) = 0;
		virtual void visit(BreakStmt* breakStmt) = 0;
		virtual void visit(ContinueStmt* continueStmt) = 0;
		virtual void visit(EmptyStmt* emptyStmt) = 0;
		virtual void visit(ExpStmt *expStmt) = 0;
		virtual void visit(IfStmt* ifStmt) = 0;
		virtual void visit(ReturnStmt* returnStmt) = 0;
		virtual void visit(WhileStmt* whileStmt) = 0;

		virtual void visit(BinaryExp* binaryExp) = 0;
		virtual void visit(FuncallExp* funcallExp) = 0;
		virtual void visit(PrimaryExp* primaryExp) = 0;
		virtual void visit(UnaryExp* unaryExp) = 0;

		virtual void visit(Type* type) = 0;
		virtual void visit(ConstantInt* constantInt) = 0;
		virtual void visit(Ident* ident) = 0;
	};
}

