
| Function                                           | 函数功能描述 | Param描述 | 返回值描述 | 实现者 |
| :------------------------------------------------- | ------------ | --------- | ---------- | ---------- |
| void computeLoopInfo(BlockBegin* func_entry_block); | 计算基本Loop信息 |函数开始block|无||
| struct DequeList* getSuccessors(BlockBegin* block); | 获取后继节点 | 当前block | 后继节点表 |  |
| struct LinearList* computeBlockOrder(BlockBegin* func_entry_block); | 计算block序号 | 开始block | 线性表：按顺序的block表 | 童义 |
| void insertBlock(BlockBegin* block, list_entry_t* work_list_head); | 插入block到worklist | 待插入的block，worklist头指针 | 无 | 童义 |
| void numberLirOp(struct LinearList* blocks); | 计算Op序号 | block线性顺序表 |            | 童义 |
| void setOpOrder(struct DequeList* op_order, BlockBegin* b); | 将b块的所有Op按照+2排序放入op_order中 | op_order：OP顺序排放，b：需要处理的块 | 无 |            |
|                            |              |               |            |            |