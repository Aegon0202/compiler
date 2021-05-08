#include <iostream>
#include "EmptyStmt.h"
#include "Ident.h"
#include "MessageVisitor.h"
#include "Exp.h"
#include "ExpStmt.h"
#include "BinaryExp.h"
#include "ValueDef.h"

using namespace std;
using namespace saltyfish;

int main() {
	vector<ValueDef::arrayAssignList_t*> a;
	ValueDef::arrayAssignList_t* b = new ValueDef::arrayAssignList_t();
	ValueDef::arrayAssignList_t* c = new ValueDef::arrayAssignList_t();
	b->list.push_back(c);
	a.push_back(b);

	return 0;
}