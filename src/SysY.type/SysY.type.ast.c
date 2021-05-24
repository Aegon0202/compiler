#include "SysY.type.ast.h"
#include "SysY.type.symtab.h"

void *TokenToAST(void *token, struct toAST *visitor);

int level;
int func_offset;
int global_data_offset;

void *toASTCompUnit(struct CompUnit *cp)
{
    struct CompUnit *head = cp;
    do
    {
        if (cp->valuetype == FUNCDEF)
        {
            toASTFuncDef(cp->value.funcdef);
        }
        else if (cp->valuetype == DECL)
        {
            //toASTDecl(cp->value.decl);
        }
        cp = cp->next;
    } while (cp != head);
    return NULL;
}

void *toASTFuncDef(struct FuncDef *funcdef)
{
    level++;
    struct FuncSymEntry *fse = newFuncSymEntry(FUNCSYMENTRY, funcdef->ident->name, funcsymtable_p->head);
    fse->returntype = funcdef->functype->typevalue;
    if (funcdef->funcfparams->funcfparam == NULL)
    {
        fse->funcparamnum = 0;
    }
    else
    {
        struct FuncFParams *head = funcdef->funcfparams;
        struct FuncFParams *fp = head;
        // int offset = 0;
        do
        {
            struct VarSymEntry *vse = newVarSymEntry(VARSYMENTRY, fp->funcfparam->ident->name, level, fse->funcparam_head);
            fse->funcparamnum++;
            vse->typevalue = fp->funcfparam->btype->typevalue;
            vse->is_const = 0;
            vse->is_array = 0;
            vse->array_dimensional_num = 0;
            vse->array_shape = NULL;
            if (fp->funcfparam->exparraydefs)
            {
                vse->is_array = 1;
                struct ExpArrayDefs *eadfs_head = fp->funcfparam->exparraydefs;
                struct ExpArrayDefs *eadfs = fp->funcfparam->exparraydefs;
                do
                {
                    vse->array_dimensional_num++;
                    eadfs = eadfs->next;
                } while (eadfs != eadfs_head);
                vse->array_shape = (int *)malloc(sizeof(int) * vse->array_dimensional_num);

                int i = 0;
                do
                {
                    IfNullElse(eadfs->exparraydef->exp, vse->array_shape[i] = 0;, ;);
                } while (eadfs != eadfs_head);
            }
        } while (fp != head);
    }
    level--;
    return NULL;
}

//int calcExpConst(struct )