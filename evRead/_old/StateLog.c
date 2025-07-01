#include "StateLog.h"

struct StateLogType* SL_Find(struct StateLogType* sl0, unsigned short st)
{
    struct StateLogType* retval = sl0;
    while ((sl0->value < st) && (sl0->next))
    {
        sl0 = sl0->next;
    }
    if (sl0->value != st)
    {
        retval = malloc (sizeof(struct StateLogType));
        retval->next = sl0->next;
        retval->value = st;
        sl0->next = retval;
    }
    return retval;
}
struct StateLogType* SL_Add(struct StateLogType* sl0, unsigned short* to, unsigned int n)
{
    if (!sl0)
    {
        sl0 = malloc (sizeof(struct StateLogType));
    }
    struct StateLogType* start = SL_Find(sl0, to[0]);
    for (int i=1; i<n; i++)
    {
        struct StateLogType* next = SL_Find(sl0, to[i]);
        SL_Transition(start, next);
        start = next;
    }
    return sl0;
}
void SL_Transition (struct StateLogType* fr, struct StateLogType* to)
{
    struct StateToType* to0 = fr->to0;
    if (!to0)
    {
        fr->to0 = malloc (sizeof(struct StateToType));
        to0 = fr->to0;
    }
    else
    {
        while (to0->to->value < to->value)
        {
            to0 = to0->next;
        }
        if (to0->)
    }
    while ()
}
struct StateLogType;
struct StateToType
{
    unsigned int frequency;
    struct StateLogType* to;
    struct StateToType* next;
};
struct StateLogType
{
    unsigned int value;
    unsigned int frequency;
    struct StateToType* to0;
    struct StateLogType* next;
};