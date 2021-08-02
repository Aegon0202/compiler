#ifndef SYSY_TARGET_ARM_H
#define SYSY_TARGET_ARM_H

#include <stdint.h>

#include "../ENUM.h"
#include "../utils/link.h"
#define PC R15
#define LR R14
#define SP R13
#define IP R12
#define FP R11
#define V8 R11
#define V7 R10
#define V6 R9
#define SB R9
#define TR R9
#define V5 R8
#define V4 R7
#define V3 R6
#define V2 R5
#define V1 R4
#define A4 R3
#define A3 R2
#define A2 R1
#define A1 R0
#define BEGIN_REG_NUM 128

struct Immi_8 {
    int8_t num;
};

struct Immi_12 {
    int16_t num;
};

struct Immi_16 {
    int16_t num;
};

struct Register {
    int type;
    int reg;
};

struct Label {
    char* label;
};

struct Operand2 {
    int type;  // IMMI_8, REGISTER
    union {
        struct Immi_8* immi_8;
        int reg;
    } Rm;
    int shift_type;  // NOP, LSL, LSR, ASR, ROR, RRX
    int shift_op;    // IMMI_8, REGISTER
    union {
        struct Immi_8* immi_8;
        int reg;
    } shift;
};

struct CondOp {
    int type;  // EQ, NE, CS, CC, MI, PL, VS, VC, HI, LS, GE, LT, GT, LE, AL
};

struct ArmIr {
    int type;
    struct CondOp* cond;
    void* op1;
    void* op2;
    void* op3;
    void* op4;
    list_entry_t ir_link;
};

void generateAllToOutFile(FILE* out_file);

#endif