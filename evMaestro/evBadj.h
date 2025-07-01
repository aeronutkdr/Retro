#ifndef _EVBADJ_H_
#define _EVBADJ_H_
#include "evPrimitives.h"

struct evBadjType
{
    PlayerType player;
    int LeftHand;
};

char evProcessBadj(char* str, struct evBadjType* pbadj);
void evDumpBadj(struct evBadjType* pbadj);

#endif /* _EVBADJ_H_ */
