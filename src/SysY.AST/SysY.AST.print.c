#include "SysY.AST.print.h"

static int identation;
#define LEVEL_IDENT 1
#define PRINT(msg, ...)                      \
    do {                                     \
        for (int i = 0; i < identation; i++) \
            fprintf(stdout, "|---");         \
        fprintf(stdout, msg, ##__VA_ARGS__); \
    } while (0)

void printASTOperand(struct Operand* operand) {
    IfNull(operand, { PRINT("NULL POINT\n"); return ; });
    switch (operand->valuetype) {
        case INTCONST:
            PRINT("INTCONST: %d\n", operand->value.intconst->value);
            break;
        case VARSYMENTRY:
            PRINT("VARSYMENTRY: %s\n", operand->value.variable->name);
            identation += LEVEL_IDENT;
            printASTVarSymEntry(operand->value.variable, 0);
            identation -= LEVEL_IDENT;
            break;
        case FUNCIMPLAST:
            PRINT("FUNCIMPLAST: %s\n", operand->value.function->function->name);
            identation += LEVEL_IDENT;
            PRINT("PARAM NUM: %d\n", operand->value.function->param_num);
            identation += LEVEL_IDENT;
            for (int i = 0; i < operand->value.function->param_num; i++) {
                printASTOperand(operand->value.function->param[i]);
            }
            identation -= LEVEL_IDENT;
            identation -= LEVEL_IDENT;
            break;
        case ARRAYIMPLAST:
            PRINT("ARRAYIMPL: %s\n", operand->value.array->array_varsymentry->name);
            identation += LEVEL_IDENT;
            PRINT("ARRAY IMPL SIZE: %d\n", operand->value.array->array_impl_size);
            identation += LEVEL_IDENT;
            for (int i = 0; i < operand->value.array->array_impl_size; i++) {
                printASTOperand(operand->value.array->array_impl[i]);
            }
            identation -= LEVEL_IDENT;
            identation -= LEVEL_IDENT;
            break;
        case EXPAST:
            printASTExpAST(operand->value.exp);
            break;
        default:
            PrintErrExit("UNKNOWN OPERAND TYPE: %d\n", operand->valuetype);
    }
}

void printASTExpAST(struct ExpAST* expast) {
    IfNull(expast, { PRINT("NULL POINT\n"); return ; });
    while (expast != NULL) {
        PRINT("EXPAST: %s\n", EnumTypeToString(expast->op));
        identation += LEVEL_IDENT;
        switch (expast->op) {
            case IFSTMT:
                PRINT("CONDITION AST: \n");
                identation += LEVEL_IDENT;
                printASTOperand(expast->op1);
                identation -= LEVEL_IDENT;
                PRINT("THEN AST: \n");
                identation += LEVEL_IDENT;
                printASTOperand(expast->op2);
                identation -= LEVEL_IDENT;
                PRINT("ELSE AST: \n");
                identation += LEVEL_IDENT;
                printASTOperand(expast->op3);
                identation -= LEVEL_IDENT;
                break;
            case WHILESTMT:
                PRINT("CONDITION AST: \n");
                identation += LEVEL_IDENT;
                printASTOperand(expast->op1);
                identation -= LEVEL_IDENT;
                PRINT("THEN AST: \n");
                identation += LEVEL_IDENT;
                printASTOperand(expast->op2);
                identation -= LEVEL_IDENT;
                break;
            case BREAKSTMT:
            case CONTINUESTMT:
                // PRINT("%s\n", EnumTypeToString(expast->op));
                break;
            case RETURNSTMT:
                IfNotNullElse(expast->op1, printASTOperand(expast->op1);, PRINT("NO RETURN VALUE"););
                break;
            case ASSIGN:
                PRINT("LVAL: \n");
                identation += LEVEL_IDENT;
                printASTOperand(expast->op1);
                identation -= LEVEL_IDENT;
                PRINT("VALUE: \n");
                identation += LEVEL_IDENT;
                printASTOperand(expast->op2);
                identation -= LEVEL_IDENT;
                break;
            case OPREAND:
                printASTOperand(expast->op1);
                break;
            case INTCONST:
                PRINT("INTCONST: %d\n", expast->op1->value.intconst->value);
                break;
            case K_ADD:
            case K_SUB:
            case K_MUL:
            case K_DIV:
            case K_MOD:
            case K_AND:
            case K_OR:
            case K_EQ:
            case K_NEQ:
            case K_LT:
            case K_LTE:
            case K_GT:
            case K_GTE:
                printASTOperand(expast->op1);
                printASTOperand(expast->op2);
                break;
            default:
                PrintErrExit("UNKNOWN EXP AST OP\n");
        }
        identation -= LEVEL_IDENT;
        expast = expast->next;
    }
}

void printASTVarSymEntry(struct VarSymEntry* varsymentry, int full) {
    IfNull(varsymentry, { PRINT("NULL POINT\n"); return ; });
    PRINT("VARSYMENTRY: %s\n", varsymentry->name);
    identation += LEVEL_IDENT;
    PRINT("LEVEL: %d\n", varsymentry->level);
    PRINT("TYPE: %s\n", EnumTypeToString(varsymentry->typevalue));

    if (varsymentry->level != 1) {
        PRINT("SIZE: %d\n", varsymentry->size);
    }
    PRINT("IS ARRAY: %s\n", varsymentry->is_array ? "TRUE" : "FALSE");
    if (varsymentry->is_array && full) {
        identation += LEVEL_IDENT;
        PRINT("ARRAY DIMENSIONAL: %d\n", varsymentry->array_dimensional_num);
        identation += LEVEL_IDENT;
        for (int i = 0; i < varsymentry->array_dimensional_num; i++) {
            printASTOperand(varsymentry->array_shape[i]);
        }
        identation -= LEVEL_IDENT;
        identation -= LEVEL_IDENT;
    }

    PRINT("IS CONST: %s\n", varsymentry->is_const ? "TRUE" : "FALSE");
    if (varsymentry->is_const && full) {
        identation += LEVEL_IDENT;
        int length = varsymentry->size / (varsymentry->typevalue == K_INT ? 4 : 1);
        for (int i = 0; i < length; i++) {
            printASTOperand(varsymentry->initval[i]);
        }
        identation -= LEVEL_IDENT;
    }
    identation -= LEVEL_IDENT;
}

void printASTFuncSymEntry(struct FuncSymEntry* funcsymentry) {
    IfNull(funcsymentry, { PRINT("NULL POINT\n"); return ; });
    PRINT("FUNCSYMENTRY: %s\n", funcsymentry->name);
    identation += LEVEL_IDENT;
    PRINT("RETURN TYPE: %s\n", EnumTypeToString(funcsymentry->returntype));
    PRINT("FUNCFPARAMS: %d\n", funcsymentry->funcparamnum);
    identation += LEVEL_IDENT;
    for (int i = 0; i < funcsymentry->funcparamnum; i++) {
        printASTVarSymEntry(funcsymentry->funcfparam[i], 1);
    }
    identation -= LEVEL_IDENT;

    PRINT("FUNCTION BODY: \n");
    identation += LEVEL_IDENT;
    printASTExpAST(funcsymentry->funcbody);
    identation -= LEVEL_IDENT;

    identation -= LEVEL_IDENT;
}

void printASTAll(struct FuncSymEntry* head) {
    struct FuncSymEntry* fse = head->next;
    while (fse != head) {
        printASTFuncSymEntry(fse);
        fse = fse->next;
    }
}
