#include "../ssa/traverse.h"
#include "../utils/IrType.h"
#include "./lifeinterval.h"
#include "blockorder.h"

#define lowBit(n) ((n) & (-n))
#define is_One(n, index) ((n) & (0x1 << (index)))
#define set_One(n, index) ((n) |= (0x1 << (index)))
#define SingleBitMapUnion(a, b) ((a) | (b))
#define SingleBitMapIntersect(a, b) ((a) & (b))
#define SingleBitMapSub(a, b) ((a) & (~(b)))
#define id_inc 2

list_entry_t* getIrListFromBlock(BlockBegin*);
int getFirstOpId(BlockBegin* block);
int getLastOpId(BlockBegin*);
struct DequeList* getBlock_kill(BlockBegin*);
struct DequeList* getBlock_gen(BlockBegin*);
struct DequeList* getBlock_in(BlockBegin*);
struct DequeList* getBlock_out(BlockBegin*);

#define le2BlockBeginNode(elem) le2struct(elem, BlockBeginNode, link)