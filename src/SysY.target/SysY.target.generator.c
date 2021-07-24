#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif

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
    return "";
}

void generator_func_head(struct FuncRegOffset* f_offset, FILE* out_file) {
    const char* name = f_offset->funcelem->name;
    Fprintf(".text\n");
    Fprintf(".align\t2\n");
    Fprintf(".global\t%s\n", name);
    Fprintf(".syntax\tunified\n");
    Fprintf(".arch armv7-r\n");
    Fprintf(".type\t%s,\t%%function\n", name);
    FprintfWithoutIdent("%s:\n", name);
    Fprintf("PUSH\t{%s}\n", reg_to_str(A4));
    Fprintf("PUSH\t{%s}\n", reg_to_str(A3));
    Fprintf("PUSH\t{%s}\n", reg_to_str(A2));
    Fprintf("PUSH\t{%s}\n", reg_to_str(A1));

    Fprintf("PUSH\t{%s,\t%s}\n", reg_to_str(FP), reg_to_str(LR));
    Fprintf("PUSH\t{%s,\t%s,\t%s,\t%s,\t%s,\t%s,\t%s}\n", reg_to_str(V1), reg_to_str(V2), reg_to_str(V3), reg_to_str(V4), reg_to_str(V5), reg_to_str(V6), reg_to_str(V7));

    Fprintf("ADD\t%s,\t%s,\t#%d\n", reg_to_str(FP), reg_to_str(SP), 32);
    Fprintf("LDR\t%s,=%d\n", reg_to_str(A1), f_offset->max_offset);
    Fprintf("ADD\t%s,%s,%s\n", reg_to_str(SP), reg_to_str(SP), reg_to_str(A1));
}

void generator_func_return(struct FuncRegOffset* f_offset, FILE* out_file) {
    Fprintf("ADD\t%s,\t%s,\t#%d\n", reg_to_str(V1), reg_to_str(FP), -32);
    Fprintf("MOV\t%s,\t%s\n", reg_to_str(SP), reg_to_str(V1));
    Fprintf("POP\t{%s,\t%s,\t%s,\t%s,\t%s,\t%s,\t%s}\n", reg_to_str(V1), reg_to_str(V2), reg_to_str(V3), reg_to_str(V4), reg_to_str(V5), reg_to_str(V6), reg_to_str(V7));
    Fprintf("POP\t{%s,\t%s}\n", reg_to_str(FP), reg_to_str(LR));
    Fprintf("ADD\t%s,%s,#%d\n", reg_to_str(SP), reg_to_str(SP), 16);
}

void generator_func_end(const char* name, FILE* out_file) {
    Fprintf(".size\t%s,\t.-%s\n", name, name);
}

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

struct travel_block_tmp_arg {
    struct FuncRegOffset* f_offset;
    FILE* out_file;
    struct DequeList* string_queue;
};

const char* block_label(const char* func_name, BASIC_BLOCK_TYPE* basic_block) {
    static char buffer[100];
    snprintf(buffer, 100, "%s_block%p", func_name, basic_block);
    return buffer;
}
const char* string_label(const char* func_name, const char* str) {
    static char buffer[100];
    snprintf(buffer, 100, "%s_str%p", func_name, str);
    return buffer;
}

void convert_operand_write(OPERAND_TYPE* op,
                           int src_reg,
                           int temp_reg,
                           struct BlockRegOffset* b_offset,
                           struct travel_block_tmp_arg* args) {
    IfNull(op, return;);
    int* offset_p = NULL;
    FILE* out_file = args->out_file;
    switch (op->type) {
        case REGISTER:
            offset_p = getLinearList(b_offset->reg_offset, op->operand.reg_idx);
            EnsureNotNull(offset_p);
            Fprintf("LDR\t%s,\t=%d\n", reg_to_str(temp_reg), *offset_p);
            Fprintf("STR\t%s,\t[%s,\t%s]\n", reg_to_str(src_reg), reg_to_str(FP), reg_to_str(temp_reg));
            break;
        default:
            PrintErrExit("error %s", EnumTypeToString(op->type));
    }
}

void convert_operand_read(OPERAND_TYPE* op,
                          int target_reg,
                          int temp_reg,
                          struct BlockRegOffset* b_offset,
                          struct travel_block_tmp_arg* args) {
    IfNull(op, return;);
    int* offset_p = NULL;
    struct Item* item;
    FILE* out_file = args->out_file;
    switch (op->type) {
        case INT:
            Fprintf("LDR\t%s,\t=%d\n", reg_to_str(target_reg), (int)op->operand.v.intValue);
            break;
        case REGISTER:
            offset_p = getLinearList(b_offset->reg_offset, op->operand.reg_idx);
            EnsureNotNull(offset_p);
            Fprintf("LDR\t%s,\t=%d\n", reg_to_str(temp_reg), *offset_p);
            Fprintf("LDR\t%s,\t[%s,\t%s]\n", reg_to_str(target_reg), reg_to_str(FP), reg_to_str(temp_reg));
            break;
        case FRAMEPOINT:
            Fprintf("LDR\t%s,\t=%d\n", reg_to_str(temp_reg), (int)op->operand.v.intValue);
            Fprintf("ADD\t%s,\t%s,\t%s\n", reg_to_str(target_reg), reg_to_str(FP), reg_to_str(temp_reg));
            break;
        case GLOBALDATA:
            Fprintf("LDR\t%s,=%s\n", reg_to_str(target_reg), ((struct VarTabElem*)op->operand.v.intValue)->name);
            break;
        case ConstSTRING:
            MALLOC_WITHOUT_DECLARE(item, struct Item, 1);
            item->key = strdup(string_label(args->f_offset->funcelem->name, op->operand.v.str));
            item->value = op->operand.v.str;
            pushFrontDequeList(args->string_queue, item);
            Fprintf("LDR\t%s,\t=%s\n", reg_to_str(target_reg), item->key);
            break;
        case FUNCID:
            Fprintf("LDR\t%s,\t=%s\n", reg_to_str(target_reg), ((struct FuncTabElem*)(op->operand.v.funcID))->name);
            break;
        case BASIC_BLOCK:
            Fprintf("LDR\t%s,\t=%s\n", reg_to_str(target_reg), block_label(args->f_offset->funcelem->name, op->operand.v.b));
            break;
        default:
            PrintErrExit("error %s", EnumTypeToString(op->type));
    }
}

#define convert_ir_func_head(type) \
    void convert_ir_##type(IR_TYPE* ir, struct BlockRegOffset* b_offset, FILE* out_file, struct travel_block_tmp_arg* args)
#define get_op_macro             \
    OPERAND_TYPE* op1 = ir->op1; \
    OPERAND_TYPE* op2 = ir->op2; \
    OPERAND_TYPE* op3 = ir->op3;

convert_ir_func_head(jump) {
    OPERAND_TYPE* op3 = ir->op3;
    convert_operand_read(op3, A1, A2, b_offset, args);
    Fprintf("BX\t%s\n", reg_to_str(A1));
}

convert_ir_func_head(branch) {
    get_op_macro;
    convert_operand_read(op1, A1, A4, b_offset, args);
    convert_operand_read(op2, A2, A4, b_offset, args);
    convert_operand_read(op3, A3, A4, b_offset, args);
    Fprintf("CMP\t%s,\t#0\n", reg_to_str(A1));
    Fprintf("IT\tEQ\n");
    Fprintf("BXEQ\t%s\n", reg_to_str(A3));
    Fprintf("BX\t%s\n", reg_to_str(A2));
}

convert_ir_func_head(return_stmt) {
    OPERAND_TYPE* op1 = ir->op1;
    convert_operand_read(op1, A1, A2, b_offset, args);
    generator_func_return(args->f_offset, out_file);
    Fprintf("BX\t%s\n", reg_to_str(LR));
}

convert_ir_func_head(load) {
    get_op_macro;
    convert_operand_read(op1, A1, A4, b_offset, args);
    convert_operand_read(op2, A2, A4, b_offset, args);
    Fprintf("LDR\t%s,\t[%s,\t%s]\n", reg_to_str(A3), reg_to_str(A1), reg_to_str(A2));
    convert_operand_write(op3, A3, A4, b_offset, args);
}

convert_ir_func_head(store) {
    get_op_macro;
    convert_operand_read(op1, A1, A4, b_offset, args);
    convert_operand_read(op2, A2, A4, b_offset, args);
    convert_operand_read(op3, A3, A4, b_offset, args);
    Fprintf("STR\t%s,\t[%s,\t%s]\n", reg_to_str(A3), reg_to_str(A2), reg_to_str(A1));
}

convert_ir_func_head(assign) {
    OPERAND_TYPE* op1 = ir->op1;
    OPERAND_TYPE* op3 = ir->op3;
    convert_operand_read(op1, A1, A3, b_offset, args);
    Fprintf("MOV\t%s,\t%s\n", reg_to_str(A2), reg_to_str(A1));
    convert_operand_write(op3, A2, A3, b_offset, args);
}

convert_ir_func_head(k_not) {
    OPERAND_TYPE* op1 = ir->op1;
    OPERAND_TYPE* op3 = ir->op3;
    convert_operand_read(op1, A1, A2, b_offset, args);

    Fprintf("CMP\t%s,\t#0\n", reg_to_str(A1));
    Fprintf("ITE\tEQ\n");
    Fprintf("MOVEQ\t%s,\t#1\n", reg_to_str(A1));
    Fprintf("MOVNE\t%s,\t#0\n", reg_to_str(A1));

    convert_operand_write(op3, A1, A2, b_offset, args);
}

#define convert_ir_bi_op(type, op)                                                       \
    convert_ir_func_head(type) {                                                         \
        get_op_macro;                                                                    \
        convert_operand_read(op1, A1, A4, b_offset, args);                               \
        convert_operand_read(op2, A2, A4, b_offset, args);                               \
        Fprintf(#op "\t%s,\t%s,\t%s\n", reg_to_str(A3), reg_to_str(A1), reg_to_str(A2)); \
        convert_operand_write(op3, A3, A4, b_offset, args);                              \
    }
convert_ir_bi_op(k_add, ADD);
convert_ir_bi_op(k_sub, SUB);
convert_ir_bi_op(k_mul, MUL);
convert_ir_bi_op(k_div, SDIV);
#undef convert_ir_bi_op

convert_ir_func_head(k_mod) {
    get_op_macro;
    convert_operand_read(op1, A1, A4, b_offset, args);
    convert_operand_read(op2, A2, A4, b_offset, args);

    Fprintf("SDIV\t%s,\t%s,\t%s\n", reg_to_str(A3), reg_to_str(A1), reg_to_str(A2));
    Fprintf("MUL\t%s,\t%s,\t%s\n", reg_to_str(A4), reg_to_str(A2), reg_to_str(A3));
    Fprintf("SUB\t%s,\t%s,\t%s\n", reg_to_str(A2), reg_to_str(A1), reg_to_str(A4));

    convert_operand_write(op3, A2, A4, b_offset, args);
}

#define convert_ir_bi_op(type, true_cond, false_cond)              \
    convert_ir_func_head(type) {                                   \
        get_op_macro;                                              \
        convert_operand_read(op1, A1, A4, b_offset, args);         \
        convert_operand_read(op2, A2, A4, b_offset, args);         \
        Fprintf("CMP\t%s,\t%s\n", reg_to_str(A1), reg_to_str(A2)); \
        Fprintf("ITE\t" #true_cond "\n");                          \
        Fprintf("MOV" #true_cond "\t%s,\t#1\n", reg_to_str(A3));   \
        Fprintf("MOV" #false_cond "\t%s,\t#0\n", reg_to_str(A3));  \
        convert_operand_write(op3, A3, A4, b_offset, args);        \
    }

convert_ir_bi_op(k_eq, EQ, NE);
convert_ir_bi_op(k_neq, NE, EQ);
convert_ir_bi_op(k_lt, LT, GE);
convert_ir_bi_op(k_lte, LE, GT);
convert_ir_bi_op(k_gt, GT, LE);
convert_ir_bi_op(k_gte, GE, LT);
#undef convert_ir_bi_op

void convert_ir_call(IR_TYPE* call_ir, struct BlockRegOffset* b_offset, FILE* out_file, struct travel_block_tmp_arg* args, struct DequeList* param_queue) {
    while (!isEmptyDequeList(param_queue)) {
        IR_TYPE* ir = popFrontDequeList(param_queue);
        OPERAND_TYPE* op2 = ir->op2;
        OPERAND_TYPE* op3 = ir->op3;
        convert_operand_read(op3, V1, V2, b_offset, args);
        switch (op2->operand.v.intValue) {
            case 0:
                Fprintf("MOV\t%s,\t%s\n", reg_to_str(A1), reg_to_str(V1));
                break;
            case 1:
                Fprintf("MOV\t%s,\t%s\n", reg_to_str(A2), reg_to_str(V1));
                break;
            case 2:
                Fprintf("MOV\t%s,\t%s\n", reg_to_str(A3), reg_to_str(V1));
                break;
            case 3:
                Fprintf("MOV\t%s,\t%s\n", reg_to_str(A4), reg_to_str(V1));
                break;
            default:
                Fprintf("PUSH\t{%s}\n", reg_to_str(V1));
                break;
        }
    }
    IR_TYPE* ir = call_ir;
    get_op_macro;
    convert_operand_read(op1, V1, V2, b_offset, args);
    Fprintf("BLX\t%s\n", reg_to_str(V1));
    convert_operand_write(op3, A1, A2, b_offset, args);
    if (op2->operand.v.intValue > 4) {
        Fprintf("LDR\t%s,=%d\n", reg_to_str(A1), (int)((4 - op2->operand.v.intValue) * INT_SIZE));
        Fprintf("ADD\t%s,\t%s,\t%s\n", reg_to_str(SP), reg_to_str(SP), reg_to_str(A1));
    }
}

void convert_block_travel(BASIC_BLOCK_TYPE* basic_block, void* args) {
    struct FuncRegOffset* f_offset = ((struct travel_block_tmp_arg*)args)->f_offset;
    FILE* out_file = ((struct travel_block_tmp_arg*)args)->out_file;
    struct BlockRegOffset* b_offset = getLinearList(f_offset->block_offsets, (size_t)basic_block);
    FprintfWithoutIdent("%s:\n", block_label(f_offset->funcelem->name, basic_block));

    struct DequeList* param_queue = newDequeList();

    list_entry_t* head = &(basic_block->ir_list->ir_link);
    list_entry_t* next = head->next;

    while (head != next) {
        IR_TYPE* ir = le2struct(next, IR_TYPE, ir_link);
        switch (ir->type) {
            case NOP:
                break;
            case PARAM:
                pushFrontDequeList(param_queue, ir);
                break;
            case CALL:
                convert_ir_call(ir, b_offset, out_file, args, param_queue);
                break;
            case JUMP:
                convert_ir_jump(ir, b_offset, out_file, args);
                break;
            case BRANCH:
                convert_ir_branch(ir, b_offset, out_file, args);
                break;
            case RETURNSTMT:
                convert_ir_return_stmt(ir, b_offset, out_file, args);
                break;
            case LOAD:
                convert_ir_load(ir, b_offset, out_file, args);
                break;
            case STORE:
                convert_ir_store(ir, b_offset, out_file, args);
                break;
            case ASSIGN:
                convert_ir_assign(ir, b_offset, out_file, args);
                break;
            case K_NOT:
                convert_ir_k_not(ir, b_offset, out_file, args);
                break;
            case K_ADD:
                convert_ir_k_add(ir, b_offset, out_file, args);
                break;
            case K_SUB:
                convert_ir_k_sub(ir, b_offset, out_file, args);
                break;
            case K_MUL:
                convert_ir_k_mul(ir, b_offset, out_file, args);
                break;
            case K_DIV:
                convert_ir_k_div(ir, b_offset, out_file, args);
                break;
            case K_MOD:
                convert_ir_k_mod(ir, b_offset, out_file, args);
                break;
            case K_EQ:
                convert_ir_k_eq(ir, b_offset, out_file, args);
                break;
            case K_NEQ:
                convert_ir_k_neq(ir, b_offset, out_file, args);
                break;
            case K_LT:
                convert_ir_k_lt(ir, b_offset, out_file, args);
                break;
            case K_LTE:
                convert_ir_k_lte(ir, b_offset, out_file, args);
                break;
            case K_GT:
                convert_ir_k_gt(ir, b_offset, out_file, args);
                break;
            case K_GTE:
                convert_ir_k_gte(ir, b_offset, out_file, args);
                break;
            default:
                PrintErrExit("Not support ir type %s", EnumTypeToString(ir->type));
        }
        next = list_next(next);
    }

    while (!isEmptyDequeList(param_queue)) {
        popFrontDequeList(param_queue);
    }
    freeDequeList(&param_queue);
}

void generator_func(struct FuncTabElem* fte, FILE* out_file) {
    if (fte->blocks == NULL) {
        return;
    }
    struct FuncRegOffset* f_offset = generatorRegOffset(fte);
    struct DequeList* string_queue = newDequeList();

    generator_func_head(f_offset, out_file);
    MALLOC(arg, struct travel_block_tmp_arg, 1);
    arg->f_offset = f_offset;
    arg->out_file = out_file;
    arg->string_queue = string_queue;
    deepTraverseSuccessorsBasicBlock(fte->blocks, convert_block_travel, arg);
    free(arg);
    generator_func_end(fte->name, out_file);

    while (!isEmptyDequeList(string_queue)) {
        struct Item* item = popFrontDequeList(string_queue);
        generator_string(item->key, item->value, out_file);
        free(item->key);
        free(item);
    }
    freeDequeList(&string_queue);
}

void generateAllToOutFile(FILE* out_file) {
    struct FuncTabElem* elem;
    for (int i = 0; i < func_table->next_func_index; i++) {
        elem = getLinearList(func_table->all_funcs, i);
        generator_func(elem, out_file);
    }
    struct BlockTabElem* b_elem = getLastDisplay(display);
    struct VarTabElem* vte = b_elem->last;
    while (vte != NULL) {
        generator_global_data(vte, out_file);
        vte = vte->link;
    }
}
