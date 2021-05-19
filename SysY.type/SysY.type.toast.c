#include "SysY.type.toast.h"
#include "SysY.type.symtab.h"

VisitorFuncImplGenerator(toAST, TokenToAST, void *)();

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
            toASTDecl(cp->value.decl);
        }
        cp = cp->next;
    } while (cp != head);
}

void *toASTFuncDef(struct FuncDef *funcdef)
{
    struct FuncSymEntry *fse = newFuncSymEntry(FUNCSYMENTRY, funcdef->ident->name, funcsymtable_p);
    fse->returntype = funcdef->functype->typevalue;
    if (funcdef->funcfparams->funcfparam == NULL)
    {
        fse->funcparamnum = 0;
        fse->funcparam = NULL;
    }
    else
    {
    }
}