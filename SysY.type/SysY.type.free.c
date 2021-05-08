#include "./SysY.type.free.h"
VisitorFuncImplGenerator(freeToken_t, freeToken, void, int recusive)(recusive);
struct freeToken_t *freeToken_p;

#define ListLikeFreeGenerator1(funcname, listype, dataname1) \
    void funcname(listype *list, int recusive)               \
    {                                                        \
        listype *head = list;                                \
        if (recusive)                                        \
        {                                                    \
            while (head->next != head)                       \
            {                                                \
                listype *tmp = head->next;                   \
                head->next = tmp->next;                      \
                free(tmp->dataname1);                        \
                free(tmp);                                   \
            }                                                \
            free(head->dataname1);                           \
        }                                                    \
        free(head);                                          \
    }

void initFreeToken()
{
    freeToken_p = malloc(sizeof(struct freeToken_t));
    EnsureNotNull(freeToken_p);
}