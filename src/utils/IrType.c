#include "./IrType.h"
int isWriteOp(IR_TYPE* ir, int op_index) {
#define WRITE_OP(num) \
    if (num == op_index) return 1;
#define READ_OP(num) \
    if (num == op_index) return 0;

    IR_OP_READ_WRITE(ir->type, READ_OP, WRITE_OP, break);

    return 0;
#undef READ_OP
#undef WRITE_OP
}
int isReadOp(IR_TYPE* ir, int op_index) {
#define WRITE_OP(num) \
    if (num == op_index) return 0;
#define READ_OP(num) \
    if (num == op_index) return 1;

    IR_OP_READ_WRITE(ir->type, READ_OP, WRITE_OP, break);

    return 0;
#undef READ_OP
#undef WRITE_OP
}