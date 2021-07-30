#ifndef SYSY_TYPE_NEW_H
#define SYSY_TYPE_NEW_H
#include "./SysY.type.def.h"

struct Keyword *newKeyword(int type, int keytype, const char *keyword);

struct IntConst *newIntConst(int type, char *number);

struct CompUnit *newCompUnit(int type, int valuetype, void *value, struct CompUnit *reducecp);

struct Decl *newDecl(int type, int valuetype, void *constdecl);

struct ConstDecl *newConstDecl(int type, struct BType *btype, struct ConstDefs *constdefs);

struct BType *newBType(int type, int typevalue);

struct ConstDefs *newConstDefs(int type, struct ConstDef *constdef, struct ConstDefs *prev);

struct ConstDef *newConstDef(int type, struct Ident *ident, struct ConstArrayDefs *constarraydefs, struct ConstInitVal *constinitval);

struct ConstArrayDefs *newConstArrayDefs(int type, struct ConstArrayDef *constarraydef, struct ConstArrayDefs *prev);

struct ConstArrayDef *newConstArrayDef(int type, struct ConstExp *constexp);

struct ConstInitVal *newConstInitVal(int type, int valuetype, void *constexp);

struct ConstInitVals *newConstInitVals(int type, struct ConstInitVal *constinitval, struct ConstInitVals *prev);

struct VarDecl *newVarDecl(int type, struct BType *btype, struct VarDefs *vardefs);

struct VarDefs *newVarDefs(int type, struct VarDef *vardef, struct VarDefs *prev);

struct VarDef *newVarDef(int type, struct Ident *ident, struct ConstArrayDefs *constarraydefs, struct InitVal *initval);

struct InitVal *newInitVal(int type, int valuetype, void *exp);

struct InitVals *newInitVals(int type, struct InitVal *initval, struct InitVals *prev);

struct FuncDef *newFuncDef(int type, struct FuncType *functype, struct Ident *ident, struct FuncFParams *funcfparams, struct Block *block);

struct FuncType *newFuncType(int type, int typevalue);

struct FuncFParams *newFuncFParams(int type, struct FuncFParam *funcfparam, struct FuncFParams *prev);

struct FuncFParam *newFuncFParam(int type, struct BType *btype, struct Ident *ident, struct ExpArrayDefs *exparraydefs);

struct ExpArrayDefs *newExpArrayDefs(int type, struct ExpArrayDef *exparraydef, struct ExpArrayDefs *prev);

struct ExpArrayDef *newExpArrayDef(int type, struct Exp *exp);

struct Block *newBlock(int type, struct BlockItems *blockitems);

struct BlockItems *newBlockItems(int type, struct BlockItem *blockitem, struct BlockItems *prev);

struct BlockItem *newBlockItem(int type, int valuetype, void *decl);

struct Stmt *newStmt(int type, int valuetype, void *assign);

struct Assign *newAssign(int type, struct LVal *lval, struct Exp *exp);

struct IfStmt *newIfStmt(int type, struct Cond *cond, struct Stmt *then, struct Stmt *otherwise);

struct WhileStmt *newWhileStmt(int type, struct Cond *cond, struct Stmt *stmt);

struct ReturnStmt *newReturnStmt(int type, struct Exp *exp);

struct Exp *newExp(int type, struct AddExp *addexp);

struct Cond *newCond(int type, struct LOrExp *lorexp);

struct LVal *newLVal(int type, int valuetype, void *ident);

struct ArrayImpl *newArrayImpl(int type, struct Ident *ident, struct ExpArrayDefs *exparraydefs);

struct PrimaryExp *newPrimaryExp(int type, int valuetype, void *exp);

struct Number *newNumber(int type, struct IntConst *intconst);

struct UnaryExp *newUnaryExp(int type, int valuetype, void *primaryexp);

struct FuncImpl *newFuncImpl(int type, struct Ident *ident, struct FuncRParams *funcrparams);

struct UnaryExps *newUnaryExps(int type, struct UnaryOp *unaryop, struct UnaryExp *unaryexp);

struct UnaryOp *newUnaryOp(int type, int typevalue);

struct FuncRParams *newFuncRParams(int type, struct FuncRParam *funcrparam, struct FuncRParams *prev);

struct FuncRParam *newFuncRParam(int type, int valuetype, void *exp);

struct MulExp *newMulExp(int type, struct MulOp *mulop, struct UnaryExp *unaryexp, struct MulExp *prev);

struct MulOp *newMulOp(int type, int typevalue);

struct AddExp *newAddExp(int type, struct AddOp *addop, struct MulExp *mulexp, struct AddExp *prev);

struct AddOp *newAddOp(int type, int typevalue);

struct RelExp *newRelExp(int type, struct RelOp *relop, struct AddExp *addexp, struct RelExp *prev);

struct RelOp *newRelOp(int type, int typevalue);

struct EqExp *newEqExp(int type, struct EqOp *eqop, struct RelExp *relexp, struct EqExp *prev);

struct EqOp *newEqOp(int type, int typevalue);

struct LAndExp *newLAndExp(int type, struct EqExp *eqexp, struct LAndExp *prev);

struct LOrExp *newLOrExp(int type, struct LAndExp *landexp, struct LOrExp *prev);

struct ConstExp *newConstExp(int type, struct AddExp *addexp);

struct Ident *newIdent(int type, const char *name);

struct String *newString(int type, const char *content);
#endif
