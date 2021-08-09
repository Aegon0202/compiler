#include <stdio.h>

#include "../ssa/ssa.h"
#include "../ssa/traverse.h"
#include "./SysY.target.arm.h"
#include "./SysY.target.offset.h"
extern YYSTYPE result;
#define Fprintf(...) fprintf(out_file, "\t"__VA_ARGS__)
#define FprintfWithoutIdent(...) fprintf(out_file, __VA_ARGS__)

extern struct FuncRegOffset* f_offset;

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

void __convert_ir_to_file(struct ArmIr* arm_ir, FILE* out_file) {
    switch (arm_ir->type) {
        case ARM_ADD:
            Fprintf("ADD\t%s,\t%s,\t%s\n", __convert_reg_to_str(arm_ir->op1), __convert_reg_to_str(arm_ir->op2), __convert_op2_to_str(arm_ir->op3));
            break;
        case ARM_SUB:
            Fprintf("SUB\t%s,\t%s,\t%s\n", __convert_reg_to_str(arm_ir->op1), __convert_reg_to_str(arm_ir->op2), __convert_op2_to_str(arm_ir->op3));
            break;
        case ARM_MUL:
            Fprintf("MUL\t%s,\t%s,\t%s\n", __convert_reg_to_str(arm_ir->op1), __convert_reg_to_str(arm_ir->op2), __convert_reg_to_str(arm_ir->op3));
            break;
        case ARM_SDIV:
            Fprintf("SDIV\t%s,\t%s,\t%s\n", __convert_reg_to_str(arm_ir->op1), __convert_reg_to_str(arm_ir->op2), __convert_reg_to_str(arm_ir->op3));
            break;
        case ARM_SMMUL:
            Fprintf("SMMUL\t%s,\t%s,\t%s\n", __convert_reg_to_str(arm_ir->op1), __convert_reg_to_str(arm_ir->op2), __convert_reg_to_str(arm_ir->op3));
            break;
        case ARM_MOV:
            Fprintf("MOV\t%s,\t%s\n", __convert_reg_to_str(arm_ir->op1), __convert_op2_to_str(arm_ir->op2));
            break;
        case ARM_MVN:
            Fprintf("MVN\t%s,\t%s\n", __convert_reg_to_str(arm_ir->op1), __convert_op2_to_str(arm_ir->op2));
            break;
        case ARM_CLZ:
            Fprintf("CLZ\t%s,\t%s\n", __convert_reg_to_str(arm_ir->op1), __convert_reg_to_str(arm_ir->op2));
            break;
        case ARM_MOVW:
            Fprintf("MOVW\t%s,\t#%d\n", __convert_reg_to_str(arm_ir->op1), ((struct Immi_16*)(arm_ir->op2))->num);
            break;
        case ARM_MOVT:
            Fprintf("MOVT\t%s,\t#%d\n", __convert_reg_to_str(arm_ir->op1), ((struct Immi_16*)(arm_ir->op2))->num);
            break;
        case ARM_MOVW_L:
            Fprintf("MOVW\t%s,\t#:lower:%s\n", __convert_reg_to_str(arm_ir->op1), ((struct Label*)(arm_ir->op2))->label);
            break;
        case ARM_MOVT_L:
            Fprintf("MOVT\t%s,\t#:upper:%s\n", __convert_reg_to_str(arm_ir->op1), ((struct Label*)(arm_ir->op2))->label);
            break;
        case ARM_CMP:
            Fprintf("CMP\t%s,\t%s\n", __convert_reg_to_str(arm_ir->op1), __convert_op2_to_str(arm_ir->op2));
            break;
        case ARM_CMN:
            Fprintf("CMN\t%s,\t%s\n", __convert_reg_to_str(arm_ir->op1), __convert_op2_to_str(arm_ir->op2));
            break;
        case ARM_B:
            Fprintf("B\t%s\n", ((struct Label*)(arm_ir->op1))->label);
            break;
        case ARM_BL:
            Fprintf("BL\t%s\n", ((struct Label*)(arm_ir->op1))->label);
            break;
        case ARM_BX:
            Fprintf("BX\t%s\n", __convert_reg_to_str(arm_ir->op1));
            break;
        case ARM_BLX:
            Fprintf("BLX\t%s\n", __convert_reg_to_str(arm_ir->op1));
            break;
        case ARM_LDR_I:
            Fprintf("LDR\t%s,\t[%s,\t#%d]\n", __convert_reg_to_str(arm_ir->op1), __convert_reg_to_str(arm_ir->op2), ((struct Immi_12*)(arm_ir->op3))->num);
            break;
        case ARM_LDR_I_PRE:
            Fprintf("LDR\t%s,\t[%s,\t#%d]!\n", __convert_reg_to_str(arm_ir->op1), __convert_reg_to_str(arm_ir->op2), ((struct Immi_12*)(arm_ir->op3))->num);
            break;
        case ARM_LDR_I_POST:
            Fprintf("LDR\t%s,\t[%s],\t#%d\n", __convert_reg_to_str(arm_ir->op1), __convert_reg_to_str(arm_ir->op2), ((struct Immi_12*)(arm_ir->op3))->num);
            break;
        case ARM_LDR_R:
            Fprintf("LDR\t%s,\t[%s,\t%s]\n", __convert_reg_to_str(arm_ir->op1), __convert_reg_to_str(arm_ir->op2), __convert_op2_to_str(arm_ir->op3));
            break;
        case ARM_LDR_R_PRE:
            Fprintf("LDR\t%s,\t[%s,\t%s]!\n", __convert_reg_to_str(arm_ir->op1), __convert_reg_to_str(arm_ir->op2), __convert_op2_to_str(arm_ir->op3));
            break;
        case ARM_LDR_R_POST:
            Fprintf("LDR\t%s,\t[%s],\t%s\n", __convert_reg_to_str(arm_ir->op1), __convert_reg_to_str(arm_ir->op2), __convert_op2_to_str(arm_ir->op3));
            break;
        case ARM_STR_I:
            Fprintf("STR\t%s,\t[%s,\t#%d]\n", __convert_reg_to_str(arm_ir->op1), __convert_reg_to_str(arm_ir->op2), ((struct Immi_12*)(arm_ir->op3))->num);
            break;
        case ARM_STR_I_PRE:
            Fprintf("STR\t%s,\t[%s,\t#%d]!\n", __convert_reg_to_str(arm_ir->op1), __convert_reg_to_str(arm_ir->op2), ((struct Immi_12*)(arm_ir->op3))->num);
            break;
        case ARM_STR_I_POST:
            Fprintf("STR\t%s,\t[%s],\t#%d\n", __convert_reg_to_str(arm_ir->op1), __convert_reg_to_str(arm_ir->op2), ((struct Immi_12*)(arm_ir->op3))->num);
            break;
        case ARM_STR_R:
            Fprintf("STR\t%s,\t[%s,\t%s]\n", __convert_reg_to_str(arm_ir->op1), __convert_reg_to_str(arm_ir->op2), __convert_op2_to_str(arm_ir->op3));
            break;
        case ARM_STR_R_PRE:
            Fprintf("STR\t%s,\t[%s,\t%s]!\n", __convert_reg_to_str(arm_ir->op1), __convert_reg_to_str(arm_ir->op2), __convert_op2_to_str(arm_ir->op3));
            break;
        case ARM_STR_R_POST:
            Fprintf("STR\t%s,\t[%s],\t%s\n", __convert_reg_to_str(arm_ir->op1), __convert_reg_to_str(arm_ir->op2), __convert_op2_to_str(arm_ir->op3));
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

void __convert_to_file(struct DequeList* block_list, FILE* out_file) {
    struct FuncTabElem* func = f_offset->funcelem;

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
    Fprintf("MOVW\t%s,\t#0x%04x\n", reg_to_str(A1), f_offset->max_offset & 0xffff);
    Fprintf("MOVT\t%s,\t#0x%04x\n", reg_to_str(A1), (f_offset->max_offset >> 16) & 0xffff);
    Fprintf("ADD\t%s,%s,%s\n", reg_to_str(SP), reg_to_str(SP), reg_to_str(A1));

    gothrough_BlockBeginNode_list(block_list, __convert_to_file_block, out_file);
}