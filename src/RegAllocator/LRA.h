#ifndef REG_ALLOCATOR_LRA
#define REG_ALLOCATOR_LRA

#include "../ssa/traverse.h"
#include "../utils/IrType.h"
#include "./allocation.h"
#include "./lifeinterval.h"
#include "blockorder.h"
#define lowBit(n) ((n) & (-n))
#define is_One(n, index) ((n) & ((long long int)0x1 << (index)))
#define set_One(n, index) ((n) |= ((long long int)0x1 << (index)))
#define SingleBitMapUnion(a, b) ((a) | (b))
#define SingleBitMapIntersect(a, b) ((a) & (b))
#define SingleBitMapSub(a, b) ((a) & (~(b)))
#define id_inc 2

extern struct LinearList* reg2Intival;
list_entry_t* getIrListFromBlock(BlockBegin*);
int getFirstOpId(BlockBegin* block);
int getLastOpId(BlockBegin*);
int get_reg_offset(int reg_num);
struct DequeList* getBlock_kill(BlockBegin*);
struct DequeList* getBlock_gen(BlockBegin*);
struct DequeList* getBlock_in(BlockBegin*);
struct DequeList* getBlock_out(BlockBegin*);
list_entry_t* getArmIrByOpid(struct DequeList* block_list, int op_id);
//usepos序列可以无序
int getFirstUsePos(Interval* interval);
int getOptimalPos(int min_split_pos, int max_split_pos, struct DequeList* blocks);
void assign_reg2interval(Interval* current, int reg);

//current 和it之间有交叉，需要将it切分，使得切分后的it与current没有交叉，切分一次

void compute_local_live_set(struct DequeList* start);
void compute_global_live_set(struct DequeList* start);
void resolve_data_flow(struct DequeList* block_list);
void gothrough_BlockBeginNode_list(struct DequeList* block_list_head, void (*func)(BlockBegin*, void*), void* args);
void gothrough_BlockBeginNode_list_reverse(struct DequeList* block_list_head, void (*func)(BlockBegin*, void*), void* args);
#define le2BlockBeginNode(elem) le2struct(elem, BlockBeginNode, link)

Interval* child_at(int reg_num, int op_id);
Interval* create_new_interval(int reg_num, Interval* parent);
void free_interval(Interval* it);
void store_reg_to_stack(Interval* it, list_entry_t* add_before_entry);
void load_reg_from_stack(Interval* it, list_entry_t* add_before_entry);
BlockBegin* getBlockBeginByOpid(int op_id, struct DequeList* blocks);
#endif