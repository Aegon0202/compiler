#ifndef UTILS_BITMP_H
#define UTILS_BITMP_H
#include <stdint.h>

struct BitMap {
    int64_t* content;
    int length;
};

struct BitMap* newBitMap(int length);
void freeBitMap(struct Bitmap* map);

// 返回 map1 和 map2 的并集
struct BitMap* unionBitMap(struct BitMap* map1, struct BitMap* map2);
// 返回 map1 和 map2 的交集
struct BitMap* intersectionBitMap(struct BitMap* map1, struct BitMap* map2);
// 返回 map1 和 map2 的差集  (map1 - map2)
struct BitMap* differenceBitMap(struct BitMap* map1, struct BitMap* map2);
// map1 := map2
void copyBitMap(struct BitMap* map1, struct BitMap* map2);
// map1 == map2 ?
int equalBitMap(struct BitMap* map1, struct BitMap* map2);
#endif
