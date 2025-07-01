#ifndef _EVDATA_H_
#define _EVDATA_H_
#include "evPrimitives.h"

struct evDataType
{
    PlayerType player;
    int Earned;
};

char evProcessData(char* str, struct evDataType* pdat);
void evDumpData(struct evDataType* pdat);

#endif /* _EVDATA_H_ */
