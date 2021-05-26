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
    IfNull(cp, return NULL;);
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
    IfNull(string, return NULL;);
    struct String *s = (struct String *)malloc(sizeof(struct String));
    EnsureNotNull(s);
    s->type = STRING;
    s->content = strdup(string->content);
    struct Operand *o = newOperand(STRING, s);
    EnsureNotNull(o);
    return o;
}

struct Operand *toASTFuncRParam(struct FuncRParam *funcrparam) {
    IfNull(funcrparam, return NULL;);
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
    IfNull(funcimpl, return NULL;);
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
    IfNull(number, return NULL;);
    struct IntConst *intconst = newIntConstAST(number->intconst->value);
    EnsureNotNull(intconst);
    struct Operand *operand = newOperand(INTCONST, intconst);
    EnsureNotNull(operand);
    return operand;
}

struct Operand *toASTIdent(struct Ident *ident) {
    IfNull(ident, return NULL;);
    struct VarSymEntry *vse = findVarInTable(ident->name);
    IfNull(vse, PrintErrExit("NOT FOUND VARIABLE %s IN SYMBOL TABLE", ident->name));
    struct Operand *operand = newOperand(VARSYMENTRY, vse);
    EnsureNotNull(operand);
    return operand;
}

struct Operand *toASTArrayImpl(struct ArrayImpl *arrayimpl) {
    IfNull(arrayimpl, return NULL;);
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
    IfNull(lval, return NULL;);
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
    IfNull(primaryexp, return NULL;);
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
    IfNull(unaryexps, return NULL;);
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
    IfNull(unaryexp, return NULL;);
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
    IfNull(mulexp, return NULL;);
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
    IfNull(addexp, return NULL;);
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
    IfNull(exp, return NULL;);
    return toASTAddExp(exp->addexp);
}

struct Operand *toASTRelExp(struct RelExp *relexp) {
    IfNull(relexp, return NULL;);
    struct RelExp *head = relexp;
    struct Operand *operand;
    head->prev->next = NULL;
    if (head->next != NULL) {
        operand = newOperand(EXPAST, newExpAST(head->next->relop->typevalue, toASTAddExp(head->addexp), toASTRelExp(head->next), NULL, NULL));
    } else {
        operand = toASTAddExp(head->addexp);
    }
    EnsureNotNull(operand);
    head->prev->next = head;
    return operand;
}

struct Operand *toASTEqExp(struct EqExp *eqexp) {
    IfNull(eqexp, return NULL;);
    struct EqExp *head = eqexp;
    struct Operand *operand;
    head->prev->next = NULL;
    if (head->next != NULL) {
        operand = newOperand(EXPAST, newExpAST(head->next->eqop->typevalue, toASTRelExp(head->relexp), toASTEqExp(head->next), NULL, NULL));
    } else {
        operand = toASTRelExp(head->relexp);
    }
    EnsureNotNull(operand);
    head->prev->next = head;
    return operand;
}

struct Operand *toASTLAndExp(struct LAndExp *landexp) {
    IfNull(landexp, return NULL;);
    struct LAndExp *head = landexp;
    struct Operand *operand;
    head->prev->next = NULL;
    if (head->next != NULL) {
        operand = newOperand(EXPAST, newExpAST(K_AND, toASTEqExp(head->eqexp), toASTLAndExp(head->next), NULL, NULL));
    } else {
        operand = toASTEqExp(head->eqexp);
    }
    EnsureNotNull(operand);
    head->prev->next = head;
    return operand;
}

struct Operand *toASTLOrExp(struct LOrExp *lorexp) {
    IfNull(lorexp, return NULL;);
    struct LOrExp *head = lorexp;
    struct Operand *operand;
    head->prev->next = NULL;
    if (head->next != NULL) {
        operand = newOperand(EXPAST, newExpAST(K_OR, toASTLAndExp(head->landexp), toASTLOrExp(head->next), NULL, NULL));
    } else {
        operand = toASTLAndExp(head->landexp);
    }
    EnsureNotNull(operand);
    head->prev->next = head;
    return operand;
}

struct Operand *toASTCond(struct Cond *cond) {
    IfNull(cond, return NULL;);
    return toASTLOrExp(cond->lorexp);
}

void *toASTInitVal(struct InitVal *initval, struct Operand **init_target, struct Operand **array_shape, int array_size, int array_dimensional_num) {
    IfNull(initval, return NULL;);
    IfNull(array_shape, return NULL;);
    // not complete
    return NULL;
}

struct VarSymEntry *toASTVarDef(struct VarDef *vardef) {
    IfNull(vardef, return NULL;);
    struct VarSymEntry *var = newVarSymEntry(vardef->ident->name, level, varsymtable_active_p->head);
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
            toASTInitVal(vardef->initval, var->initval, var->array_shape, var->size, var->array_dimensional_num);
        } else {
            if (vardef->initval->valuetype == INITVALS) {
                PrintErrExit("TO AST VAR DECL INTI VAL TYPE ERROR\n");
            }
            var->initval[0] = toASTExp(vardef->initval->value.exp);
        }
    }
    return var;
}

void *toASTVarDecl(struct VarDecl *vardecl) {
    IfNull(vardecl, return NULL;);
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

void *toASTConstInitVal(struct ConstInitVal *initval, struct Operand **init_target, struct Operand **array_shape, int array_size, int array_dimensional_num) {
    IfNull(initval, return NULL;);
    IfNull(array_shape, return NULL;);
    // not complete
    return NULL;
}

struct VarSymEntry *toASTConstDef(struct ConstDef *constdef) {
    IfNull(constdef, return NULL;);
    struct VarSymEntry *var = newVarSymEntry(constdef->ident->name, level, varsymtable_active_p->head);
    var->size = 1;
    var->is_const = 0;
    if (constdef->constarraydefs != NULL) {
        var->is_array = 1;
        var->array_dimensional_num = 0;
        struct ConstArrayDefs *head = constdef->constarraydefs;
        struct ConstArrayDefs *exparraydefs = constdef->constarraydefs;
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
    IfNull(constdef->constinitval, PrintErrExit("CONST DECL NOT HAVE CONST INIT VALUE"));
    var->initval = (struct Operand **)malloc(sizeof(struct Operand *) * var->size);
    EnsureNotNull(var->initval);
    if (var->is_array) {
        toASTConstInitVal(constdef->constinitval, var->initval, var->array_shape, var->size, var->array_dimensional_num);
    } else {
        if (constdef->constinitval->valuetype == CONSTINITVALS) {
            PrintErrExit("TO AST VAR DECL INTI VAL TYPE ERROR\n");
        }
        var->initval[0] = newOperand(INTCONST, newIntConstAST(calcConstConstExp(constdef->constinitval->value.constexp)));
    }

    return var;
}

void *toASTConstDecl(struct ConstDecl *constdecl) {
    IfNull(constdecl, return NULL;);
    int type_value = constdecl->btype->typevalue;
    struct ConstDefs *head = constdecl->constdefs;
    struct ConstDefs *constdefs = head;
    do {
        struct VarSymEntry *vse = toASTConstDef(constdefs->constdef);
        vse->typevalue = type_value;
        vse->size *= 4;
        constdefs = constdefs->next;
    } while (constdefs != head);
    return NULL;
}

void *toASTDecl(struct Decl *decl) {
    IfNull(decl, return NULL;);
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

void *toASTFuncFParams(struct FuncFParams *funcfparams, struct FuncSymEntry *fse) {
    IfNull(funcfparams, return NULL;);
    IfNull(fse, return NULL;);
    if (funcfparams->funcfparam == NULL) {
        fse->funcparamnum = 0;
    } else {
        struct FuncFParams *head = funcfparams;
        struct FuncFParams *fp = head;
        // int offset = 0;
        do {
            fse->funcparamnum++;
            fp = fp->next;
        } while (fp != head);

        fse->funcfparam = (struct VarSymEntry **)malloc(sizeof(struct VarSymEntry *) * fse->funcparamnum);
        int i = 0;
        do {
            struct VarSymEntry *vse = newVarSymEntry(fp->funcfparam->ident->name, level, varsymtable_active_p->head);
            fse->funcfparam[i] = vse;
            i++;
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
                    IfNullElse(eadfs->exparraydef->exp, vse->array_shape[i] = NULL;, vse->array_shape[i] = toASTExp(eadfs->exparraydef->exp););
                    eadfs = eadfs->next;
                } while (eadfs != eadfs_head);
            }
        } while (fp != head);
    }
    return NULL;
}

struct ExpAST *toASTIfStmt(struct IfStmt *ifstmt) {
    IfNull(ifstmt, return NULL;);
    struct Operand *cond = toASTCond(ifstmt->cond);
    EnsureNotNull(cond);
    struct Operand *then_o = NULL;
    struct Operand *other_o = NULL;
    struct ExpAST *then = toASTStmt(ifstmt->then);
    IfNotNull(then, then_o = newOperand(EXPAST, then););
    struct ExpAST *other = toASTStmt(ifstmt->otherwise);
    IfNotNull(other, other_o = newOperand(EXPAST, other););
    return newExpAST(IFSTMT, cond, then_o, other_o, NULL);
}

struct ExpAST *toASTWhileStmt(struct WhileStmt *whilestmt) {
    IfNull(whilestmt, return NULL;);
    struct Operand *cond = toASTCond(whilestmt->cond);
    EnsureNotNull(cond);
    struct Operand *then_o = NULL;
    struct ExpAST *then = toASTStmt(whilestmt->stmt);
    IfNotNull(then, then_o = newOperand(EXPAST, then););
    return newExpAST(IFSTMT, cond, then_o, NULL, NULL);
}

struct ExpAST *toASTStmt(struct Stmt *stmt) {
    IfNull(stmt, return NULL;);
    struct Operand *operand;
    switch (stmt->valuetype) {
        case ASSIGN:
            return newExpAST(ASSIGN, toASTLVal(stmt->value.assign->lval), toASTExp(stmt->value.assign->exp), NULL, NULL);
        case BLOCK:
            return toASTBlock(stmt->value.block);
        case IFSTMT:
            return toASTIfStmt(stmt->value.ifstmt);
        case WHILESTMT:
            return toASTWhileStmt(stmt->value.whilestmt);
        case RETURNSTMT:
            return newExpAST(RETURNSTMT, toASTExp(stmt->value.returnstmt->exp), NULL, NULL, NULL);
        case EXP:
            operand = toASTExp(stmt->value.exp);
            if (operand->valuetype == EXPAST) {
                return operand->value.exp;
            }
            return newExpAST(OPREAND, operand, NULL, NULL, NULL);
        case BREAKSTMT:
            return newExpAST(BREAKSTMT, NULL, NULL, NULL, NULL);
        case CONTINUESTMT:
            return newExpAST(CONTINUESTMT, NULL, NULL, NULL, NULL);

        default:
            PrintErrExit("TO AST STMT UNKNOWN VALUE TYPE\n");
    }
}

struct ExpAST *toASTBlockItem(struct BlockItem *blockitem) {
    IfNull(blockitem, return NULL;);
    switch (blockitem->valuetype) {
        case DECL:
            return toASTDecl(blockitem->value.decl);
        case STMT:
            return toASTStmt(blockitem->value.stmt);

        default:
            PrintErrExit("TO AST BLOCK ITEM UNKNOWN VALUE TYPE\n");
    }
}

struct ExpAST *toASTBlockItems(struct BlockItems *blockitems) {
    IfNull(blockitems, return NULL;);
    struct BlockItems *head = blockitems;

    head->prev->next = NULL;
    struct ExpAST *expast = toASTBlockItem(blockitems->blockitem);
    struct ExpAST *rest = toASTBlockItems(head->next);
    head->prev->next = head;

    if (expast != NULL) {
        expast->next = rest;
    } else {
        expast = rest;
    }
    return expast;
}

struct ExpAST *toASTBlock(struct Block *block) {
    IfNull(block, return NULL;);
    level++;
    struct ExpAST *expast = toASTBlockItems(block->blockitems);
    removeVarFromSymTable(level);
    level--;
    return expast;
}

struct FuncSymEntry *toASTFuncDef(struct FuncDef *funcdef) {
    IfNull(funcdef, return NULL;);
    level++;
    struct FuncSymEntry *fse = newFuncSymEntry(funcdef->ident->name, funcsymtable_p->head);
    fse->returntype = funcdef->functype->typevalue;
    toASTFuncFParams(funcdef->funcfparams, fse);
    fse->funcbody = toASTBlock(funcdef->block);
    removeVarFromSymTable(level);
    level--;
    return NULL;
}
