#include <stdio.h>

#include "../ssa/ssa.h"
#include "../ssa/traverse.h"
#include "./SysY.target.arm.h"
extern YYSTYPE result;
extern struct DequeList* string_queue;
#define Fprintf(...) fprintf(out_file, "\t"__VA_ARGS__)
#define FprintfWithoutIdent(...) fprintf(out_file, __VA_ARGS__)

void generator_string(const char* name, const char* content, FILE* out_file) {
    Fprintf(".section\t.rodata\n");
    Fprintf(".align\t2\n");
    FprintfWithoutIdent("%s:\n", name);
    *(strchr(content, 0) - 1) = '\0';
    Fprintf(".ascii\t\"%s\\000\"\n", content + 1);
}

void generator_global_data(struct VarTabElem* elem, FILE* out_file) {
    if (elem->level != 0) {
        return;
    }
    Fprintf(".data\n");
    Fprintf(".align\t2\n");
    Fprintf(".global\t%s\n", elem->name);
    Fprintf(".type\t%s,\t%%object\n", elem->name);
    Fprintf(".size\t%s,\t%d\n", elem->name, elem->size);
    FprintfWithoutIdent("%s:\n", elem->name);
    if (elem->const_init_value != NULL) {
        for (int i = 0; i < elem->size / INT_SIZE; i++) {
            Fprintf(".word\t%d\n", elem->const_init_value[i]);
        }
    } else {
        Fprintf(".space\t%d\n", elem->size);
    }
}

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
    return "";
}

const char* __convert_reg_to_str(struct Register* reg) {
    if (reg->type != PHISICAL) {
        PrintErrExit("not upport reg type %d, %s", reg->type, EnumTypeToString(reg->type));
    }
    return reg_to_str(reg->reg);
}

const char* __convert_op2_to_str(struct Operand2* op2) {
    static char buffer[64];
    int offset = 0;
    if (op2->type == REGISTER) {
        offset += snprintf(buffer, 64, "%s", __convert_reg_to_str(op2->Rm.reg));
    } else {
        offset += snprintf(buffer, 64, "#%d", op2->Rm.immi_8->num);
        return buffer;
    }
    if (op2->shift_type != NOP) {
        char* type = NULL;
        switch (op2->shift_type) {
            case LSL:
                type = "LSL";
                break;
            case LSR:
                type = "LSR";
                break;
            case ASR:
                type = "ASR";
                break;
            case ROR:
                type = "ROR";
                break;
            case RRX:
                type = "RRX";
                break;
            default:
                PrintErrExit("not support shift type");
        }
        offset += snprintf(buffer + offset, 64 - offset, ",\t%s", type);
        if (op2->shift_op == REGISTER) {
            offset += snprintf(buffer + offset, 64 - offset, "\t%s", __convert_reg_to_str(op2->shift.reg));
        } else {
            offset += snprintf(buffer + offset, 64 - offset, "\t#%d", op2->shift.immi_8->num);
        }
    }
    return buffer;
}

const char* __convert_cond_to_str(struct CondOp* cond) {
    switch (cond->type) {
        case EQ:
            return "EQ";
        case NE:
            return "NE";
        case HI:
            return "HI";
        case LS:
            return "LS";
        case GE:
            return "GE";
        case LT:
            return "LT";
        case GT:
            return "GT";
        case LE:
            return "LE";
        case AL:
            return "";
        default:
            PrintErrExit("not support cond type");
    }
    return "";
}

void __convert_ir_to_file(struct ArmIr* arm_ir, FILE* out_file) {
    switch (arm_ir->type) {
        case ARM_ADD:
            Fprintf("ADD%s\t%s,\t%s,\t%s\n", __convert_cond_to_str(arm_ir->cond), __convert_reg_to_str(arm_ir->op1), __convert_reg_to_str(arm_ir->op2), __convert_op2_to_str(arm_ir->op3));
            break;
        case ARM_SUB:
            Fprintf("SUB%s\t%s,\t%s,\t%s\n", __convert_cond_to_str(arm_ir->cond), __convert_reg_to_str(arm_ir->op1), __convert_reg_to_str(arm_ir->op2), __convert_op2_to_str(arm_ir->op3));
            break;
        case ARM_MUL:
            Fprintf("MUL%s\t%s,\t%s,\t%s\n", __convert_cond_to_str(arm_ir->cond), __convert_reg_to_str(arm_ir->op1), __convert_reg_to_str(arm_ir->op2), __convert_reg_to_str(arm_ir->op3));
            break;
        case ARM_SDIV:
            Fprintf("SDIV%s\t%s,\t%s,\t%s\n", __convert_cond_to_str(arm_ir->cond), __convert_reg_to_str(arm_ir->op1), __convert_reg_to_str(arm_ir->op2), __convert_reg_to_str(arm_ir->op3));
            break;
        case ARM_SMMUL:
            Fprintf("SMMUL%s\t%s,\t%s,\t%s\n", __convert_cond_to_str(arm_ir->cond), __convert_reg_to_str(arm_ir->op1), __convert_reg_to_str(arm_ir->op2), __convert_reg_to_str(arm_ir->op3));
            break;
        case ARM_MOV:
            Fprintf("MOV%s\t%s,\t%s\n", __convert_cond_to_str(arm_ir->cond), __convert_reg_to_str(arm_ir->op1), __convert_op2_to_str(arm_ir->op2));
            break;
        case ARM_MVN:
            Fprintf("MVN%s\t%s,\t%s\n", __convert_cond_to_str(arm_ir->cond), __convert_reg_to_str(arm_ir->op1), __convert_op2_to_str(arm_ir->op2));
            break;
        case ARM_CLZ:
            Fprintf("CLZ%s\t%s,\t%s\n", __convert_cond_to_str(arm_ir->cond), __convert_reg_to_str(arm_ir->op1), __convert_reg_to_str(arm_ir->op2));
            break;
        case ARM_MOVW:
            Fprintf("MOVW%s\t%s,\t#0x%04x\n", __convert_cond_to_str(arm_ir->cond), __convert_reg_to_str(arm_ir->op1), ((struct Immi_16*)(arm_ir->op2))->num & 0xffff);
            break;
        case ARM_MOVT:
            Fprintf("MOVT%s\t%s,\t#0x%04x\n", __convert_cond_to_str(arm_ir->cond), __convert_reg_to_str(arm_ir->op1), ((struct Immi_16*)(arm_ir->op2))->num & 0xffff);
            break;
        case ARM_MOVW_L:
            Fprintf("MOVW%s\t%s,\t#:lower16:%s\n", __convert_cond_to_str(arm_ir->cond), __convert_reg_to_str(arm_ir->op1), ((struct Label*)(arm_ir->op2))->label);
            break;
        case ARM_MOVT_L:
            Fprintf("MOVT%s\t%s,\t#:upper16:%s\n", __convert_cond_to_str(arm_ir->cond), __convert_reg_to_str(arm_ir->op1), ((struct Label*)(arm_ir->op2))->label);
            break;
        case ARM_CMP:
            Fprintf("CMP%s\t%s,\t%s\n", __convert_cond_to_str(arm_ir->cond), __convert_reg_to_str(arm_ir->op1), __convert_op2_to_str(arm_ir->op2));
            break;
        case ARM_CMN:
            Fprintf("CMN%s\t%s,\t%s\n", __convert_cond_to_str(arm_ir->cond), __convert_reg_to_str(arm_ir->op1), __convert_op2_to_str(arm_ir->op2));
            break;
        case ARM_B:
            Fprintf("B%s\t%s\n", __convert_cond_to_str(arm_ir->cond), ((struct Label*)(arm_ir->op1))->label);
            break;
        case ARM_BL:
            Fprintf("BL%s\t%s\n", __convert_cond_to_str(arm_ir->cond), ((struct Label*)(arm_ir->op1))->label);
            break;
        case ARM_BX:
            Fprintf("BX%s\t%s\n", __convert_cond_to_str(arm_ir->cond), __convert_reg_to_str(arm_ir->op1));
            break;
        case ARM_BLX:
            Fprintf("BLX%s\t%s\n", __convert_cond_to_str(arm_ir->cond), __convert_reg_to_str(arm_ir->op1));
            break;
        case ARM_LDR_I:
            Fprintf("LDR%s\t%s,\t[%s,\t#%d]\n", __convert_cond_to_str(arm_ir->cond), __convert_reg_to_str(arm_ir->op1), __convert_reg_to_str(arm_ir->op2), ((struct Immi_12*)(arm_ir->op3))->num);
            break;
        case ARM_LDR_I_PRE:
            Fprintf("LDR%s\t%s,\t[%s,\t#%d]!\n", __convert_cond_to_str(arm_ir->cond), __convert_reg_to_str(arm_ir->op1), __convert_reg_to_str(arm_ir->op2), ((struct Immi_12*)(arm_ir->op3))->num);
            break;
        case ARM_LDR_I_POST:
            Fprintf("LDR%s\t%s,\t[%s],\t#%d\n", __convert_cond_to_str(arm_ir->cond), __convert_reg_to_str(arm_ir->op1), __convert_reg_to_str(arm_ir->op2), ((struct Immi_12*)(arm_ir->op3))->num);
            break;
        case ARM_LDR_R:
            Fprintf("LDR%s\t%s,\t[%s,\t%s]\n", __convert_cond_to_str(arm_ir->cond), __convert_reg_to_str(arm_ir->op1), __convert_reg_to_str(arm_ir->op2), __convert_op2_to_str(arm_ir->op3));
            break;
        case ARM_LDR_R_PRE:
            Fprintf("LDR%s\t%s,\t[%s,\t%s]!\n", __convert_cond_to_str(arm_ir->cond), __convert_reg_to_str(arm_ir->op1), __convert_reg_to_str(arm_ir->op2), __convert_op2_to_str(arm_ir->op3));
            break;
        case ARM_LDR_R_POST:
            Fprintf("LDR%s\t%s,\t[%s],\t%s\n", __convert_cond_to_str(arm_ir->cond), __convert_reg_to_str(arm_ir->op1), __convert_reg_to_str(arm_ir->op2), __convert_op2_to_str(arm_ir->op3));
            break;
        case ARM_STR_I:
            Fprintf("STR%s\t%s,\t[%s,\t#%d]\n", __convert_cond_to_str(arm_ir->cond), __convert_reg_to_str(arm_ir->op1), __convert_reg_to_str(arm_ir->op2), ((struct Immi_12*)(arm_ir->op3))->num);
            break;
        case ARM_STR_I_PRE:
            Fprintf("STR%s\t%s,\t[%s,\t#%d]!\n", __convert_cond_to_str(arm_ir->cond), __convert_reg_to_str(arm_ir->op1), __convert_reg_to_str(arm_ir->op2), ((struct Immi_12*)(arm_ir->op3))->num);
            break;
        case ARM_STR_I_POST:
            Fprintf("STR%s\t%s,\t[%s],\t#%d\n", __convert_cond_to_str(arm_ir->cond), __convert_reg_to_str(arm_ir->op1), __convert_reg_to_str(arm_ir->op2), ((struct Immi_12*)(arm_ir->op3))->num);
            break;
        case ARM_STR_R:
            Fprintf("STR%s\t%s,\t[%s,\t%s]\n", __convert_cond_to_str(arm_ir->cond), __convert_reg_to_str(arm_ir->op1), __convert_reg_to_str(arm_ir->op2), __convert_op2_to_str(arm_ir->op3));
            break;
        case ARM_STR_R_PRE:
            Fprintf("STR%s\t%s,\t[%s,\t%s]!\n", __convert_cond_to_str(arm_ir->cond), __convert_reg_to_str(arm_ir->op1), __convert_reg_to_str(arm_ir->op2), __convert_op2_to_str(arm_ir->op3));
            break;
        case ARM_STR_R_POST:
            Fprintf("STR%s\t%s,\t[%s],\t%s\n", __convert_cond_to_str(arm_ir->cond), __convert_reg_to_str(arm_ir->op1), __convert_reg_to_str(arm_ir->op2), __convert_op2_to_str(arm_ir->op3));
            break;
        case ARM_PUSH:
        case ARM_POP:
            break;
        case ARM_LABEL:
            FprintfWithoutIdent("%s:\n", ((struct Label*)(arm_ir->op1))->label);
            break;
        default:
            PrintErrExit("not support arm ir type %d, %s", arm_ir->type, EnumTypeToString(arm_ir->type));
    }
    if (arm_ir->type == ARM_PUSH) {
        fprintf(out_file, "\tPUSH\t");
        int PUSH_DEQUE_NUM = sizeDequeList(arm_ir->op1);
        for (int INDEX = 0; INDEX < PUSH_DEQUE_NUM; INDEX++) {
            struct Register* reg = getDequeList(arm_ir->op1, INDEX);
            if (INDEX == 0) {
                fprintf(out_file, "{%s", __convert_reg_to_str(reg));
            } else {
                fprintf(out_file, ",\t%s", __convert_reg_to_str(reg));
            }
        }
        fprintf(out_file, "}\n");
    }
    if (arm_ir->type == ARM_POP) {
        fprintf(out_file, "\tPOP\t");
        int PUSH_DEQUE_NUM = sizeDequeList(arm_ir->op1);
        for (int INDEX = 0; INDEX < PUSH_DEQUE_NUM; INDEX++) {
            struct Register* reg = getDequeList(arm_ir->op1, INDEX);
            if (INDEX == 0) {
                fprintf(out_file, "{%s", __convert_reg_to_str(reg));
            } else {
                fprintf(out_file, ",\t%s", __convert_reg_to_str(reg));
            }
        }
        fprintf(out_file, "}\n");
    }
}

void __convert_to_file_block(BlockBegin* block, void* args) {
    FILE* out_file = (FILE*)args;
    list_entry_t* arm_head = &block->block->arm_ir_list->ir_link;
    list_entry_t* arm_elem = list_next(arm_head);
    while (arm_head != arm_elem) {
        struct ArmIr* arm_ir = le2struct(arm_elem, struct ArmIr, ir_link);
        arm_elem = list_next(arm_elem);
        __convert_ir_to_file(arm_ir, out_file);
    }
}

void __convert_to_file(struct FuncTabElem* func, struct DequeList* block_list, FILE* out_file) {
    Fprintf(".text\n");
    Fprintf(".align\t2\n");
    Fprintf(".global\t%s\n", func->name);
    Fprintf(".syntax\tunified\n");
    Fprintf(".arch armv7ve\n");
    Fprintf(".arm\n");
    Fprintf(".type\t%s,\t%%function\n", func->name);
    FprintfWithoutIdent("%s:\n", func->name);
    Fprintf("PUSH\t{%s}\n", reg_to_str(A4));
    Fprintf("PUSH\t{%s}\n", reg_to_str(A3));
    Fprintf("PUSH\t{%s}\n", reg_to_str(A2));
    Fprintf("PUSH\t{%s}\n", reg_to_str(A1));

    Fprintf("PUSH\t{%s,\t%s}\n", reg_to_str(FP), reg_to_str(LR));
    Fprintf("PUSH\t{%s,\t%s,\t%s,\t%s,\t%s,\t%s,\t%s}\n", reg_to_str(V1), reg_to_str(V2), reg_to_str(V3), reg_to_str(V4), reg_to_str(V5), reg_to_str(V6), reg_to_str(V7));

    Fprintf("ADD\t%s,\t%s,\t#%d\n", reg_to_str(FP), reg_to_str(SP), 32);
    Fprintf("MOVW\t%s,\t#0x%04x\n", reg_to_str(A1), func->var_offset_end & 0xffff);
    Fprintf("MOVT\t%s,\t#0x%04x\n", reg_to_str(A1), (func->var_offset_end >> 16) & 0xffff);
    Fprintf("ADD\t%s,%s,%s\n", reg_to_str(SP), reg_to_str(SP), reg_to_str(A1));

    gothrough_BlockBeginNode_list(block_list, __convert_to_file_block, out_file);

    Fprintf(".size\t%s,\t.-%s\n", func->name, func->name);
}

void generateGlobalToOutFile(FILE* out_file) {
    struct BlockTabElem* b_elem = getLastDisplay(display);
    struct VarTabElem* vte = b_elem->last;
    while (vte != NULL) {
        generator_global_data(vte, out_file);
        vte = vte->link;
    }
    while (!isEmptyDequeList(string_queue)) {
        struct Item* item = popFrontDequeList(string_queue);
        generator_string(item->key, item->value, out_file);
        free(item->key);
        free(item);
    }
}

static const char* __reg_to_str(struct Register* reg) {
    static char buf[64];
    if (reg->type == PHISICAL) {
        snprintf(buf, 64, " phi: %s", __convert_reg_to_str(reg));
    } else {
        snprintf(buf, 64, " reg: %%%d", reg->reg);
    }
    return buf;
}

static const char* __op2_to_str(struct Operand2* op2) {
    static char buffer[64];
    int offset = 0;
    offset += snprintf(buffer, 64, " op2: ");
    if (op2->type == REGISTER) {
        offset += snprintf(buffer + offset, 64 - offset, "%s", __reg_to_str(op2->Rm.reg));
    } else {
        offset += snprintf(buffer + offset, 64 - offset, "$%d", op2->Rm.immi_8->num);
        return buffer;
    }
    if (op2->shift_type != NOP) {
        char* type = NULL;
        switch (op2->shift_type) {
            case LSL:
                type = "LSL";
                break;
            case LSR:
                type = "LSR";
                break;
            case ASR:
                type = "ASR";
                break;
            case ROR:
                type = "ROR";
                break;
            case RRX:
                type = "RRX";
                break;
            default:
                PrintErrExit("not support shift type");
        }
        offset += snprintf(buffer + offset, 64 - offset, ", %s", type);
        if (op2->shift_op == REGISTER) {
            offset += snprintf(buffer + offset, 64 - offset, " %s", __reg_to_str(op2->shift.reg));
        } else {
            offset += snprintf(buffer + offset, 64 - offset, " $%d", op2->shift.immi_8->num);
        }
    }
    return buffer;
}

void __print_arm_ir(struct ArmIr* arm_ir) {
    printf("id: %-6d", arm_ir->id);
    switch (arm_ir->type) {
        case ARM_ADD:
            printf("op: %8s%-4s", "ADD", __convert_cond_to_str(arm_ir->cond));
            printf("op1: %-20s", __reg_to_str(arm_ir->op1));
            printf("op2: %-20s", __reg_to_str(arm_ir->op2));
            printf("op3: %-20s", __op2_to_str(arm_ir->op3));
            printf("\n");
            break;
        case ARM_SUB:
            printf("op: %8s%-4s", "SUB", __convert_cond_to_str(arm_ir->cond));
            printf("op1: %-20s", __reg_to_str(arm_ir->op1));
            printf("op2: %-20s", __reg_to_str(arm_ir->op2));
            printf("op3: %-20s", __op2_to_str(arm_ir->op3));
            printf("\n");
            break;
        case ARM_MUL:
            printf("op: %8s%-4s", "MUL", __convert_cond_to_str(arm_ir->cond));
            printf("op1: %-20s", __reg_to_str(arm_ir->op1));
            printf("op2: %-20s", __reg_to_str(arm_ir->op2));
            printf("op3: %-20s", __reg_to_str(arm_ir->op3));
            printf("\n");
            break;
        case ARM_SDIV:
            printf("op: %8s%-4s", "SDIV", __convert_cond_to_str(arm_ir->cond));
            printf("op1: %-20s", __reg_to_str(arm_ir->op1));
            printf("op2: %-20s", __reg_to_str(arm_ir->op2));
            printf("op3: %-20s", __reg_to_str(arm_ir->op3));
            printf("\n");
            break;
        case ARM_SMMUL:
            printf("op: %8s%-4s", "SMMUL", __convert_cond_to_str(arm_ir->cond));
            printf("op1: %-20s", __reg_to_str(arm_ir->op1));
            printf("op2: %-20s", __reg_to_str(arm_ir->op2));
            printf("op3: %-20s", __reg_to_str(arm_ir->op3));
            printf("\n");
            break;
        case ARM_MOV:
            printf("op: %8s%-4s", "MOV", __convert_cond_to_str(arm_ir->cond));
            printf("op1: %-20s", __reg_to_str(arm_ir->op1));
            printf("op2: %-20s", __op2_to_str(arm_ir->op2));
            printf("\n");
            break;
        case ARM_MVN:
            printf("op: %8s%-4s", "MVN", __convert_cond_to_str(arm_ir->cond));
            printf("op1: %-20s", __reg_to_str(arm_ir->op1));
            printf("op2: %-20s", __op2_to_str(arm_ir->op2));
            printf("\n");
            break;
        case ARM_CLZ:
            printf("op: %8s%-4s", "CLZ", __convert_cond_to_str(arm_ir->cond));
            printf("op1: %-20s", __reg_to_str(arm_ir->op1));
            printf("op2: %-20s", __reg_to_str(arm_ir->op2));
            printf("\n");
            break;
        case ARM_MOVW:
            printf("op: %8s%-4s", "MOVW", __convert_cond_to_str(arm_ir->cond));
            printf("op1: %-20s", __reg_to_str(arm_ir->op1));
            printf("op2: $%-20d", ((struct Immi_16*)(arm_ir->op2))->num);
            printf("\n");
            break;
        case ARM_MOVT:
            printf("op: %8s%-4s", "MOVT", __convert_cond_to_str(arm_ir->cond));
            printf("op1: %-20s", __reg_to_str(arm_ir->op1));
            printf("op2: $%-20d", ((struct Immi_16*)(arm_ir->op2))->num);
            printf("\n");
            break;
        case ARM_MOVW_L:
            printf("op: %8s%-4s", "MOVW", __convert_cond_to_str(arm_ir->cond));
            printf("op1: %-20s", __reg_to_str(arm_ir->op1));
            printf("op2: #:lower:%-12s", ((struct Label*)(arm_ir->op2))->label);
            printf("\n");
            break;
        case ARM_MOVT_L:
            printf("op: %8s%-4s", "MOVT", __convert_cond_to_str(arm_ir->cond));
            printf("op1: %-20s", __reg_to_str(arm_ir->op1));
            printf("op2: #:upper:%-12s", ((struct Label*)(arm_ir->op2))->label);
            printf("\n");
            break;
        case ARM_CMP:
            printf("op: %8s%-4s", "CMP", __convert_cond_to_str(arm_ir->cond));
            printf("op1: %-20s", __reg_to_str(arm_ir->op1));
            printf("op2: %-20s", __op2_to_str(arm_ir->op2));
            printf("\n");
            break;
        case ARM_CMN:
            printf("op: %8s%-4s", "CMP", __convert_cond_to_str(arm_ir->cond));
            printf("op1: %-20s", __reg_to_str(arm_ir->op1));
            printf("op2: %-20s", __op2_to_str(arm_ir->op2));
            printf("\n");
            break;
        case ARM_B:
            printf("op: %8s%-4s", "B", __convert_cond_to_str(arm_ir->cond));
            printf("op1: #%-18s", ((struct Label*)(arm_ir->op1))->label);
            printf("\n");
            break;
        case ARM_BL:
            printf("op: %8s%-4s", "BL", __convert_cond_to_str(arm_ir->cond));
            printf("op1: #%-18s", ((struct Label*)(arm_ir->op1))->label);
            printf("\n");
            break;
        case ARM_BX:
            printf("op: %8s%-4s", "BX", __convert_cond_to_str(arm_ir->cond));
            printf("op1: %-20s", __reg_to_str(arm_ir->op1));
            printf("\n");
            break;
        case ARM_BLX:
            printf("op: %8s%-4s", "BLX", __convert_cond_to_str(arm_ir->cond));
            printf("op1: %-20s", __reg_to_str(arm_ir->op1));
            printf("\n");
            break;
        case ARM_LDR_I:
            printf("op: %8s%-4s", "LDR", __convert_cond_to_str(arm_ir->cond));
            printf("op1: %-20s", __reg_to_str(arm_ir->op1));
            printf("op2: %-20s", __reg_to_str(arm_ir->op2));
            printf("op3: $%-20d", ((struct Immi_12*)(arm_ir->op3))->num);
            printf("\n");
            break;
        case ARM_LDR_I_PRE:
            printf("op: %8s%-4s", "LDR_PRE", __convert_cond_to_str(arm_ir->cond));
            printf("op1: %-20s", __reg_to_str(arm_ir->op1));
            printf("op2: %-20s", __reg_to_str(arm_ir->op2));
            printf("op3: $%-20d", ((struct Immi_12*)(arm_ir->op3))->num);
            printf("\n");
            break;
        case ARM_LDR_I_POST:
            printf("op: %8s%-4s", "LDR_POST", __convert_cond_to_str(arm_ir->cond));
            printf("op1: %-20s", __reg_to_str(arm_ir->op1));
            printf("op2: %-20s", __reg_to_str(arm_ir->op2));
            printf("op3: $%-20d", ((struct Immi_12*)(arm_ir->op3))->num);
            printf("\n");
            break;
        case ARM_LDR_R:
            printf("op: %8s%-4s", "LDR", __convert_cond_to_str(arm_ir->cond));
            printf("op1: %-20s", __reg_to_str(arm_ir->op1));
            printf("op2: %-20s", __reg_to_str(arm_ir->op2));
            printf("op3: %-20s", __op2_to_str(arm_ir->op3));
            printf("\n");
            break;
        case ARM_LDR_R_PRE:
            printf("op: %8s%-4s", "LDR_PRE", __convert_cond_to_str(arm_ir->cond));
            printf("op1: %-20s", __reg_to_str(arm_ir->op1));
            printf("op2: %-20s", __reg_to_str(arm_ir->op2));
            printf("op3: %-20s", __op2_to_str(arm_ir->op3));
            printf("\n");
            break;
        case ARM_LDR_R_POST:
            printf("op: %8s%-4s", "LDR_POST", __convert_cond_to_str(arm_ir->cond));
            printf("op1: %-20s", __reg_to_str(arm_ir->op1));
            printf("op2: %-20s", __reg_to_str(arm_ir->op2));
            printf("op3: %-20s", __op2_to_str(arm_ir->op3));
            printf("\n");
            break;
        case ARM_STR_I:
            printf("op: %8s%-4s", "STR", __convert_cond_to_str(arm_ir->cond));
            printf("op1: %-20s", __reg_to_str(arm_ir->op1));
            printf("op2: %-20s", __reg_to_str(arm_ir->op2));
            printf("op3: $%-20d", ((struct Immi_12*)(arm_ir->op3))->num);
            printf("\n");
            break;
        case ARM_STR_I_PRE:
            printf("op: %8s%-4s", "STR_PRE", __convert_cond_to_str(arm_ir->cond));
            printf("op1: %-20s", __reg_to_str(arm_ir->op1));
            printf("op2: %-20s", __reg_to_str(arm_ir->op2));
            printf("op3: $%-20d", ((struct Immi_12*)(arm_ir->op3))->num);
            printf("\n");
            break;
        case ARM_STR_I_POST:
            printf("op: %8s%-4s", "STR_POST", __convert_cond_to_str(arm_ir->cond));
            printf("op1: %-20s", __reg_to_str(arm_ir->op1));
            printf("op2: %-20s", __reg_to_str(arm_ir->op2));
            printf("op3: $%-20d", ((struct Immi_12*)(arm_ir->op3))->num);
            printf("\n");
            break;
        case ARM_STR_R:
            printf("op: %8s%-4s", "STR", __convert_cond_to_str(arm_ir->cond));
            printf("op1: %-20s", __reg_to_str(arm_ir->op1));
            printf("op2: %-20s", __reg_to_str(arm_ir->op2));
            printf("op3: %-20s", __op2_to_str(arm_ir->op3));
            printf("\n");
            break;
        case ARM_STR_R_PRE:
            printf("op: %8s%-4s", "STR_PRE", __convert_cond_to_str(arm_ir->cond));
            printf("op1: %-20s", __reg_to_str(arm_ir->op1));
            printf("op2: %-20s", __reg_to_str(arm_ir->op2));
            printf("op3: %-20s", __op2_to_str(arm_ir->op3));
            printf("\n");
            break;
        case ARM_STR_R_POST:
            printf("op: %8s%-4s", "STR_POST", __convert_cond_to_str(arm_ir->cond));
            printf("op1: %-20s", __reg_to_str(arm_ir->op1));
            printf("op2: %-20s", __reg_to_str(arm_ir->op2));
            printf("op3: %-20s", __op2_to_str(arm_ir->op3));
            printf("\n");
            break;
        case ARM_PUSH:
        case ARM_POP:
            break;
        case ARM_LABEL:
            printf("%s:\n", ((struct Label*)(arm_ir->op1))->label);
            break;
        default:
            PrintErrExit("not support arm ir type %d, %s", arm_ir->type, EnumTypeToString(arm_ir->type));
    }
    if (arm_ir->type == ARM_PUSH) {
        printf("op: %8s%-4s", "PUSH", __convert_cond_to_str(arm_ir->cond));
        printf("regs: ");
        int PUSH_DEQUE_NUM = sizeDequeList(arm_ir->op1);
        for (int INDEX = 0; INDEX < PUSH_DEQUE_NUM; INDEX++) {
            struct Register* reg = getDequeList(arm_ir->op1, INDEX);
            printf("%-10s", __reg_to_str(reg));
        }
        printf("\n");
    }
    if (arm_ir->type == ARM_POP) {
        printf("op: %8s%-4s", "POP", __convert_cond_to_str(arm_ir->cond));
        printf("regs: ");
        int PUSH_DEQUE_NUM = sizeDequeList(arm_ir->op1);
        for (int INDEX = 0; INDEX < PUSH_DEQUE_NUM; INDEX++) {
            struct Register* reg = getDequeList(arm_ir->op1, INDEX);
            printf("%-10s", __reg_to_str(reg));
        }
        printf("\n");
    }
}

void __print_arm_ir_block(BlockBegin* block, void* args) {
    list_entry_t* arm_head = &block->block->arm_ir_list->ir_link;
    list_entry_t* arm_elem = list_next(arm_head);
    while (arm_head != arm_elem) {
        struct ArmIr* arm_ir = le2struct(arm_elem, struct ArmIr, ir_link);
        arm_elem = list_next(arm_elem);
        __print_arm_ir(arm_ir);
    }
}