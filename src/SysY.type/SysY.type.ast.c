#include "SysY.type.ast.h"

#include "../SysY.AST/SysY.AST.def.h"
#include "../SysY.AST/SysY.AST.new.h"
#include "SysY.AST.def.h"
#include "SysY.type.calcConst.h"
#include "SysY.type.symtab.h"
void *TokenToAST(void *token, struct toAST *visitor);

int level;
int func_offset;
int global_data_offset;

void *toASTCompUnit(struct CompUnit *cp) {
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

void *toASTDecl(struct Decl *decl) {
    int is_const = 0;
    if (decl->valuetype == VARDECL) {
        int type_value = decl->value.vardecl->btype->typevalue;
        struct VarDefs *head = decl->value.vardecl->vardefs;
        struct VarDefs *vardefs = head;
        do {
            struct VarDef *vardef = vardefs->vardef;
            struct VarSymEntry *var = newVarSymEntry(vardef->ident->name, level, varsymtable_active_p);
            var->type = type_value;
            var->is_const = 0;
            if (vardef->constarraydefs != NULL) {
                var->is_array = 1;
                struct ConstArrayDefs *head = vardef->constarraydefs;
                struct ConstArrayDefs *exparraydefs = vardef->constarraydefs;
                do {
                    var->array_dimensional_num++;
                    exparraydefs = exparraydefs->next;
                } while (head != exparraydefs);
                var->array_shape = (struct ExpAST **)malloc(sizeof(struct ExpAST *) * var->array_dimensional_num);
                int i = 0;
                var->size = 4;
                do {
                    int s = calcConstConstExp(exparraydefs->constarraydef->constexp);
                    var->array_shape[i] = newExpAST(INTCONST, newOperand(INTCONST, newIntConstAST(s)), NULL, NULL, NULL);
                    var->size *= s;
                    exparraydefs = exparraydefs->next;
                    i++;
                } while (head != exparraydefs);
            }
            vardefs = vardefs->next;
        } while (vardefs != head);
    }
}

struct FuncSymEntry *toASTFuncDef(struct FuncDef *funcdef) {
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
                vse->array_shape = (int *)malloc(sizeof(int) * vse->array_dimensional_num);

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
