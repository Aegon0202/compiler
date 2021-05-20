#include "SysY.type.ast.hpp"
#include "SysY.type.symtab.hpp"

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
}

void *toASTFuncDef(struct FuncDef *funcdef)
{
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
        do
        {
            struct VarSymEntry *vse = newVarSymEntry(fp->funcfparam->btype->typevalue, fp->funcfparam->ident->name, level, fse->funcparam_head);
        } while (fp != head);
    }
}