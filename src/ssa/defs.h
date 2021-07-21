#include <stdint.h>

#define offset(type, member) ((size_t) & (((type *)(void *)0)->member))

typedef int bool;

/* to_struct - get the struct from a ptr
 * @ptr:    a struct pointer of member
 * @type:   the type of the struct this is embedded in
 * @member: the name of the member within the struct
 * */
#define to_struct(ptr, type, member) \
    ((type *)((char *)(ptr)-offset(type, member)))
