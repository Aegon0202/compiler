#ifndef SYSY_SYMTAB_CALCCONST_H
#define SYSY_SYMTAB_CALCCONST_H
#include "./SysY.symtab.def.h"

int calcConstArrayImpl(struct ArrayImpl *arrayimpl);
int calcConstIdent(struct Ident *ident);
int calcConstLVal(struct LVal *lval);
int calcConstExp(struct Exp *exp);
int calcConstNumber(struct Number *number);
int calcConstPrimaryExp(struct PrimaryExp *primaryexp);
int calcConstUnaryExps(struct UnaryExps *unaryexps);
int calcConstUnaryExp(struct UnaryExp *unaryexp);
int calcConstMulExp(struct MulExp *mulexp);
int calcConstAddExp(struct AddExp *addexp);
int calcConstConstExp(struct ConstExp *const_exp);
#endif