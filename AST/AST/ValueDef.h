#pragma once
#include <string>
#include <memory>

#include "Exp.h"
#include "Ident.h"

using namespace std;

#include <vector>
#include <memory>

#include "Exp.h"
#include "ASTUnit.h"


namespace saltyfish {
	class ASTVisitor;

	class ValueDef
		:virtual public ASTUnit
	{
	public:
		class ValueDefBitFields {
		public:
			unsigned hasAssign : 1;
			unsigned isArray : 1;
			unsigned isArrayFirstDimEmpty : 1;
			unsigned isArrayAssign : 1;
		} bitFields;

		struct ArrayList {
			enum Type {
				Val, List
			} type;
			union {
				unique_ptr<Exp> val;
				vector<ArrayList*> list;
			};
		public:
			ArrayList() {}
			~ArrayList() {}
		};

		unique_ptr<Ident> ident;
		vector<unique_ptr<Exp>> arrayDimList;
		unique_ptr<Exp> exp;

	public:
		ValueDef();
		ValueDef(unique_ptr<Ident> ident, vector<unique_ptr<Exp>> arrayDimList);
		ValueDef(unique_ptr<Ident> ident, vector<unique_ptr<Exp>> arrayDimList, unique_ptr<Exp> exp);
		ValueDef(unique_ptr<Ident> ident, vector<unique_ptr<Exp>> arrayDimList,location loc);
		ValueDef(unique_ptr<Ident> ident, vector<unique_ptr<Exp>> arrayDimList, unique_ptr<Exp> exp,location loc);
		bool hasAssign();
		bool isArrayAssign();
		bool isArray();
		bool isArrayFirstDimEmpty();
		void setHasAssign(bool b);
		void setIsArrayAssign(bool b);
		void setIsArray(bool b);
		void setIsArrayFirstDimEmpty(bool b);
		virtual void accept(ASTVisitor& visitor);
	};
}