#ifndef _EVSTARTSUB_H_
#define _EVSTARTSUB_H_
#include "evPrimitives.h"

#define NAME_SIZE (100)
struct evStartSubType
{
    PlayerType player;
    char Fullname[NAME_SIZE+1];
    int Home;
    int Order;
    int Position;
};

char evProcessStartSub(char* str, struct evStartSubType* pss);
void evDumpStartSub(struct evStartSubType* pss);

#endif /* _EVSTARTSUB_H_ */
