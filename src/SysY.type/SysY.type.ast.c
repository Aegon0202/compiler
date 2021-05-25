#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif

#include "SysY.type.ast.h"

#include "../SysY.AST/SysY.AST.def.h"
#include "../SysY.AST/SysY.AST.new.h"
#include "SysY.type.calcConst.h"
#include "SysY.type.def.h"
#include "SysY.type.symtab.h"

int level;
int func_offset;
int global_data_offset;

void *toASTCompUnit(struct CompUnit *cp) {
    // not complele
    struct CompUnit *head = cp;
    level = 0;
    do {
        if (cp->valuetype == FUNCDEF) {
            toASTFuncDef(cp->value.funcdef);
        } else if (cp->valuetype == DECL) {
            toASTDecl(cp->value.decl);
        }
        cp = cp->next;
    } while (cp != head);
    return NULL;
}

struct Operand *toASTString(struct String *string) {
    struct String *s = (struct String *)malloc(sizeof(struct String));
    EnsureNotNull(s);
    s->type = STRING;
    s->content = strdup(string->content);
    struct Operand *o = newOperand(STRING, s);
    EnsureNotNull(o);
    return o;
}

struct Operand *toASTFuncRParam(struct FuncRParam *funcrparam) {
    switch (funcrparam->valuetype) {
        case EXP:
            return toASTExp(funcrparam->value.exp);
        case STRING:
            return toASTString(funcrparam->value.string);
        default:
            PrintErrExit("TO AST FUNCR PARAM UNKNOWN VALUE TYPE\n");
    }
}

struct Operand *toASTFuncImpl(struct FuncImpl *funcimpl) {
    struct FuncSymEntry *fse = findFuncInTable(funcimpl->ident->name);
    IfNull(fse, PrintErrExit("NOT FOUND FUNCTION NAME"););
    struct Operand **param = NULL;
    if (fse->funcparamnum > 0) {
        param = (struct Operand **)malloc(sizeof(struct Operand *) * fse->funcparamnum);
        struct FuncRParams *head = funcimpl->funcrparams;
        struct FuncRParams *funcrparams = funcimpl->funcrparams;
        for (int i = 0; i < fse->funcparamnum; i++, funcrparams = funcrparams->next) {
            if (i > 0 && funcrparams == head) {
                PrintErrExit("TO AST FUNC IMPL PARAM PROVIDE NOT MATCH NEED\n");
            }
            param[i] = toASTFuncRParam(funcrparams->funcrparam);
        }
    }
    struct FuncImplAST *func_impl_ast = newFuncImplAST(fse, fse->funcparamnum, param);
    EnsureNotNull(func_impl_ast);
    struct Operand *operand = newOperand(FUNCIMPLAST, func_impl_ast);
    return operand;
}

struct Operand *toASTNumber(struct Number *number) {
    struct IntConst *intconst = newIntConstAST(number->intconst->value);
    EnsureNotNull(intconst);
    struct Operand *operand = newOperand(INTCONST, intconst);
    EnsureNotNull(operand);
    return operand;
}

struct Operand *toASTIdent(struct Ident *ident) {
    struct VarSymEntry *vse = findVarInTable(ident->name);
    IfNull(vse, PrintErrExit("NOT FOUND VARIABLE %s IN SYMBOL TABLE", ident->name));
    struct Operand *operand = newOperand(VARSYMENTRY, vse);
    EnsureNotNull(operand);
    return operand;
}

struct Operand *toASTArrayImpl(struct ArrayImpl *arrayimpl) {
    struct VarSymEntry *vse = findVarInTable(arrayimpl->ident->name);
    IfNull(vse, PrintErrExit("NOT FOUND VARIABLE %s IN SYMBOL TABLE", arrayimpl->ident->name));
    struct Operand **impl = (struct Operand **)malloc(sizeof(struct Operand *) * vse->array_dimensional_num);
    EnsureNotNull(impl);
    int impl_num = 0;
    struct ExpArrayDefs *head = arrayimpl->exparraydefs;
    struct ExpArrayDefs *exparraydefs = arrayimpl->exparraydefs;
    do {
        impl_num++;
        exparraydefs = exparraydefs->next;
    } while (head != exparraydefs);
    int i = 0;
    for (i = 0; i < impl_num; i++, exparraydefs = exparraydefs->next) {
        impl[i] = toASTExp(exparraydefs->exparraydef->exp);
    }
    for (; i < vse->array_dimensional_num; i++) {
        impl[i] = newOperand(INTCONST, newIntConstAST(0));
    }
    struct ArrayImplAST *array_impl_ast = newArrayImplAST(vse, impl_num, impl);
    EnsureNotNull(array_impl_ast);
    struct Operand *operand = newOperand(ARRAYIMPLAST, array_impl_ast);
    EnsureNotNull(operand);
    return operand;
}

struct Operand *toASTLVal(struct LVal *lval) {
    switch (lval->valuetype) {
        case IDENT:
            return toASTIdent(lval->value.ident);
        case ARRAYIMPL:
            return toASTArrayImpl(lval->value.arrayimpl);
        default:
            PrintErrExit("TO AST LVAL UNKNOWN VALUE TYPE\n");
    }
}

struct Operand *toASTPrimaryExp(struct PrimaryExp *primaryexp) {
    switch (primaryexp->valuetype) {
        case EXP:
            return toASTExp(primaryexp->value.exp);
        case LVAL:
            return toASTLVal(primaryexp->value.lval);
        case NUMBER:
            return toASTNumber(primaryexp->value.number);
        default:
            PrintErrExit("TO AST PRIMARY EXP UNKNOWN VALUE TYPE\n");
    }
}

struct Operand *toASTUnaryExps(struct UnaryExps *unaryexps) {
    struct Operand *op1 = toASTUnaryExp(unaryexps->unaryexp);
    IfNull(unaryexps->unaryexp, return op1;);
    switch (unaryexps->unaryop->type) {
        case K_ADD:
            return newOperand(EXPAST, newExpAST(K_ADD, newOperand(INTCONST, newIntConstAST(0)), op1, NULL, NULL));
        case K_SUB:
            return newOperand(EXPAST, newExpAST(K_SUB, newOperand(INTCONST, newIntConstAST(0)), op1, NULL, NULL));
        case K_NOT:
            return newOperand(EXPAST, newExpAST(K_NOT, op1, NULL, NULL, NULL));
        default:
            PrintErrExit("TO AST UNARY EXPS UNKNOWN VALUE TYPE\n");
    }
}

struct Operand *toASTUnaryExp(struct UnaryExp *unaryexp) {
    switch (unaryexp->valuetype) {
        case PRIMARYEXP:
            return toASTPrimaryExp(unaryexp->value.primaryexp);
        case FUNCIMPL:
            return toASTFuncImpl(unaryexp->value.funcimpl);
        case UNARYEXPS:
            return toASTUnaryExps(unaryexp->value.unaryexps);
        default:
            PrintErrExit("TO AST UNARY EXP UNKNOWN VALUE TYPE\n");
    }
}

struct Operand *toASTMulExp(struct MulExp *mulexp) {
    struct MulExp *head = mulexp;
    struct Operand *exp_ast;
    head->prev->next = NULL;
    if (head->next != NULL) {
        exp_ast = newOperand(EXPAST, newExpAST(head->next->mulop->typevalue, toASTUnaryExp(head->unaryexp), toASTMulExp(head->next), NULL, NULL));
    } else {
        exp_ast = toASTUnaryExp(head->unaryexp);
    }
    EnsureNotNull(exp_ast);
    head->prev->next = head;
    return exp_ast;
}

struct Operand *toASTAddExp(struct AddExp *addexp) {
    struct AddExp *head = addexp;
    struct Operand *exp_ast;
    head->prev->next = NULL;
    if (head->next != NULL) {
        exp_ast = newOperand(EXPAST, newExpAST(head->next->addop->typevalue, toASTMulExp(head->mulexp), toASTAddExp(head->next), NULL, NULL));
    } else {
        exp_ast = toASTMulExp(head->mulexp);
    }
    EnsureNotNull(exp_ast);
    head->prev->next = head;
    return exp_ast;
}

struct Operand *toASTExp(struct Exp *exp) {
    return toASTAddExp(exp->addexp);
}

struct Operand **toASTInitVal(struct InitVal *initval, struct Operand **array_shape, int array_size, int array_dimensional_num) {
    // not complete
    switch (initval->valuetype) {
        case EXP:
            struct Operand **operand = (struct Operand **)malloc(sizeof(struct Operand *));
            EnsureNotNull(operand);
            operand[0] = toASTExp(initval->value.exp);
            return operand;
        case INITVALS:
            int i;
            int length = calcConstOperand(array_shape[0]);
            struct Operand **init = (struct Operand **)malloc(sizeof(struct Operand *) * array_size);
            if (initval->value.initvals == NULL) {
                for (i = 0; i < array_size; i++) {
                    init[i] = newOperand(INTCONST, newIntConstAST(0));
                }
            } else {
                for (i = 0; i < array_size;) {
                }
            }
            return init;
        default:
            PrintErrExit("TO AST INITVAL UNKNOWN VALUE TYPE\n");
    }
}

struct VarSymEntry *toASTVarDef(struct VarDef *vardef) {
    // not complele
    struct VarSymEntry vse_stack;
    struct VarSymEntry *var = &vse_stack;  //newVarSymEntry(vardef->ident->name, level, varsymtable_active_p->head);
    var->size = 1;
    var->is_const = 0;
    if (vardef->constarraydefs != NULL) {
        var->is_array = 1;
        var->array_dimensional_num = 0;
        struct ConstArrayDefs *head = vardef->constarraydefs;
        struct ConstArrayDefs *exparraydefs = vardef->constarraydefs;
        do {
            var->array_dimensional_num++;
            exparraydefs = exparraydefs->next;
        } while (head != exparraydefs);
        var->array_shape = (struct Operand **)malloc(sizeof(struct Operand *) * var->array_dimensional_num);
        EnsureNotNull(var->array_shape);
        int i = 0;
        do {
            int s = calcConstConstExp(exparraydefs->constarraydef->constexp);
            var->array_shape[i] = newOperand(INTCONST, newIntConstAST(s));
            var->size *= s;
            exparraydefs = exparraydefs->next;
            i++;
        } while (head != exparraydefs);
    }
    if (vardef->initval != NULL) {
        var->initval = (struct Operand **)malloc(sizeof(struct Operand *) * var->size);
        EnsureNotNull(var->initval);
        if (var->is_array) {
            var->initval = ;
        } else {
            if (vardef->initval->valuetype == INITVALS) {
                PrintErrExit("TO AST VAR DECL INTI VAL TYPE ERROR\n");
            }
            var->initval[0] = toASTExp(vardef->initval->value.exp);
        }
    }
    return NULL;
}

void *toASTVarDecl(struct VarDecl *vardecl) {
    // not complele
    int type_value = vardecl->btype->typevalue;
    struct VarDefs *head = vardecl->vardefs;
    struct VarDefs *vardefs = head;
    do {
        struct VarSymEntry *vse = toASTVarDef(vardefs->vardef);
        vse->typevalue = type_value;
        vse->size *= 4;
        vardefs = vardefs->next;
    } while (vardefs != head);
    return NULL;
}

void *toASTDecl(struct Decl *decl) {
    int is_const = 0;
    switch (decl->valuetype) {
        case VARDECL:
            return toASTVarDecl(decl->value.vardecl);
        case CONSTDECL:
            return toASTConstDecl(decl->value.constdecl);
        default:
            PrintErrExit("TO AST DECL UNKNOWN VALUE TYPE\n");
    }
    return NULL;
}

struct FuncSymEntry *toASTFuncDef(struct FuncDef *funcdef) {
    // not complele
    level++;
    struct FuncSymEntry *fse = newFuncSymEntry(funcdef->ident->name, funcsymtable_p->head);
    fse->returntype = funcdef->functype->typevalue;
    if (funcdef->funcfparams->funcfparam == NULL) {
        fse->funcparamnum = 0;
    } else {
        struct FuncFParams *head = funcdef->funcfparams;
        struct FuncFParams *fp = head;
        // int offset = 0;
        do {
            struct VarSymEntry *vse = newVarSymEntry(fp->funcfparam->ident->name, level, fse->funcparam_head);
            fse->funcparamnum++;
            vse->typevalue = fp->funcfparam->btype->typevalue;
            vse->is_const = 0;
            vse->is_array = 0;
            vse->array_dimensional_num = 0;
            vse->array_shape = NULL;
            if (fp->funcfparam->exparraydefs) {
                vse->is_array = 1;
                struct ExpArrayDefs *eadfs_head = fp->funcfparam->exparraydefs;
                struct ExpArrayDefs *eadfs = fp->funcfparam->exparraydefs;
                do {
                    vse->array_dimensional_num++;
                    eadfs = eadfs->next;
                } while (eadfs != eadfs_head);
                vse->array_shape = (struct Operand **)malloc(sizeof(struct Operand *) * vse->array_dimensional_num);
                EnsureNotNull(vse->array_shape);
                int i = 0;
                do {
                    IfNullElse(eadfs->exparraydef->exp, vse->array_shape[i] = 0;, ;);
                } while (eadfs != eadfs_head);
            }
        } while (fp != head);
    }
    level--;
    return NULL;
}
