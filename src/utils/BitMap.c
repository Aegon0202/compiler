#include "./BitMap.h"

#include <assert.h>

#include "./Malloc.h"

#define LENGTH_TO_SIZE(length) (((length) >> 6) + 1)

struct BitMap* newBitMap(int length) {
    MALLOC(map, struct BitMap, 1);
    map->length = length;
    int size = LENGTH_TO_SIZE(length);
    MALLOC_WITHOUT_DECLARE(map->content, uint64_t, size);
    return map;
}
void freeBitMap(struct BitMap* map) {
    free(map->content);
    free(map);
}

uint64_t bitmap_mask(int index) {
    return ((uint64_t)0x1) << (index & 0x3f);
}

int bitmap_address(int index) {
    return index >> 6;
}

void setBitMap(struct BitMap* map, int index) {
    assert(index < map->length);
    map->content[bitmap_address(index)] |= bitmap_mask(index);
}

void clearBitMap(struct BitMap* map, int index) {
    assert(index < map->length);
    map->content[bitmap_address(index)] &= ~bitmap_mask(index);
}

int getBitMap(struct BitMap* map, int index) {
    assert(index < map->length);
    return !!(map->content[bitmap_address(index)] & bitmap_mask(index));
}

// 返回 map1 和 map2 的并集
struct BitMap* unionBitMap(struct BitMap* map1, struct BitMap* map2) {
    assert(map1->length == map2->length);
    struct BitMap* map = newBitMap(map1->length);

    int size = LENGTH_TO_SIZE(map->length);
    for (int i = 0; i < size; i++) {
        map->content[i] = map1->content[i] | map2->content[i];
    }
    return map;
}
// 返回 map1 和 map2 的交集
struct BitMap* intersectionBitMap(struct BitMap* map1, struct BitMap* map2) {
    assert(map1->length == map2->length);
    struct BitMap* map = newBitMap(map1->length);

    int size = LENGTH_TO_SIZE(map->length);
    for (int i = 0; i < size; i++) {
        map->content[i] = map1->content[i] & map2->content[i];
    }
    return map;
}
// 返回 map1 和 map2 的差集  (map1 - map2)
struct BitMap* differenceBitMap(struct BitMap* map1, struct BitMap* map2) {
    assert(map1->length == map2->length);
    struct BitMap* map = newBitMap(map1->length);

    int size = LENGTH_TO_SIZE(map->length);
    for (int i = 0; i < size; i++) {
        map->content[i] = map1->content[i] & ~(map2->content[i]);
    }
    return map;
}
// map1 := map2
void copyBitMap(struct BitMap* map1, struct BitMap* map2) {
    assert(map1->length == map2->length);

    int size = LENGTH_TO_SIZE(map1->length);
    for (int i = 0; i < size; i++) {
        map1->content[i] = map2->content[i];
    }
}
// map1 == map2 ?
int equalBitMap(struct BitMap* map1, struct BitMap* map2) {
    assert(map1->length == map2->length);

    int size = LENGTH_TO_SIZE(map1->length);
    for (int i = 0; i < size; i++) {
        if (map1->content[i] != map2->content[i]) {
            return 0;
        }
    }
    return 1;
}

int low_index(uint64_t content) {
    assert(content);
    content = content & (-content);
    int j = 0;
    while ((content >>= 1) != 0) {
        j++;
    }
    return j;
}

int getNextSetBitMap(struct BitMap* map, int from) {
    assert(0 <= from && from < map->length);
    int size = LENGTH_TO_SIZE(map->length);
    int f_index = bitmap_address(from);

    uint64_t t_mask = from & 0x3f;
    t_mask = (((uint64_t)0x1) << (t_mask + 1)) - 1;

    uint64_t content = map->content[f_index] & ~t_mask;
    if (content) {
        return (f_index << 6) + low_index(content);
    }
    for (f_index++; f_index < size; f_index++) {
        content = map->content[f_index];
        if (content) {
            return (f_index << 6) + low_index(content);
        }
    }
    return -1;
}