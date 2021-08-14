#ifndef LRA_MOVE_RESOLVER
#define LRA_MOVE_RESOLVER

#include "../utils/DequeList.h"
#include "../utils/link.h"

struct MoveSolver {
    struct DequeList* from_to_queue;
    struct LinearList* block_register;
    list_entry_t* add_before_entry;
};

typedef struct Interval Interval;

struct MoveSolver* newMoveSolver();
void freeMoveSolver(struct MoveSolver* ms);
void setAddBeforeEntry(list_entry_t* before, struct MoveSolver* mr);
void addIntervalMap(Interval* from_interval, Interval* to_interval, struct MoveSolver* mr);
void solveMove(struct MoveSolver* mr);

#endif