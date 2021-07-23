#include <stdio.h>

#include "../SysY.symtab/SysY.symtab.def.h"
#include "../ssa/traverse.h"
#include "./SysY.target.arm.h"
#include "./SysY.target.offset.h"
extern YYSTYPE result;
#define Fprintf(...) fprintf(out_file, "\t"__VA_ARGS__)
#define FprintfWithoutIdent(...) fprintf(out_file, __VA_ARGS__)

const char* reg_to_str(int reg) {
    switch (reg) {
        case R0:
            return "r0";
        case R1:
            return "r1";
        case R2:
            return "r2";
        case R3:
            return "r3";
        case R4:
            return "r4";
        case R5:
            return "r5";
        case R6:
            return "r6";
        case R7:
            return "r7";
        case R8:
            return "r8";
        case R9:
            return "r9";
        case R10:
            return "r10";
        case R11:
            return "fp";
        case R12:
            return "ip";
        case R13:
            return "sp";
        case R14:
            return "lr";
        case R15:
            return "pc";
        default:
            PrintErrExit("unknown reg");
    }
}

void generator_func_head(const char* name, FILE* out_file) {
    Fprintf(".text\n");
    Fprintf(".align 2\n");
    Fprintf(".global %s\n", name);
    Fprintf(".type %s,%%function\n", name);
    FprintfWithoutIdent("%s:\n", name);
    Fprintf("push {%s, %s, %s, %s, %s, %s}\n", reg_to_str(R4), reg_to_str(R3), reg_to_str(R2), reg_to_str(R1), reg_to_str(FP), reg_to_str(LR));
}

void generator_func_end(const char* name, FILE* out_file) {
    Fprintf(".size %s,.-%s\n", name, name);
}

void generator_global_data(struct VarTabElem* elem, FILE* out_file) {
    if (elem->level != 0) {
        return;
    }
    Fprintf(".data\n");
    Fprintf(".align 2\n");
    Fprintf(".global %s\n", elem->name);
    Fprintf(".type %s,%%object\n", elem->name);
    Fprintf(".size %s,%d\n", elem->name, elem->size);
    FprintfWithoutIdent("%s:\n", elem->name);
    if (elem->const_init_value != NULL) {
        for (int i = 0; i < elem->size / INT_SIZE; i++) {
            Fprintf(".word %x\n", elem->const_init_value[i]);
        }
    } else {
        Fprintf(".space %d\n", elem->size);
    }
}

void convertBlockActive(struct BlockActive* b_active) {}

void generator_func(struct FuncTabElem* fte, FILE* out_file) {
    struct FuncRegOffset* f_offset = generatorRegOffset(fte);
    generator_func_head(fte->name, out_file);
}

void convertFuncsToArm(FILE* out_file) {
    struct LinkedTable* visited = newLinkedTable(string_equal);
    struct DequeList* queue = newDequeList();
    struct FuncTabELem* func = getFuncTabElemByName("main", func_table);
    pushFrontDequeList(queue, func);
    setLinkedTable(visited, func, func);
    while (!isEmptyDequeList(queue)) {
        func = popBackDequeList(queue);
        struct FuncRegOffset* f_offset = generatorRegOffset(func);
    }
}
