#ifndef _EVRRESPADJ_H_
#define _EVRRESPADJ_H_
#include "evPrimitives.h"

struct RRespadjType
{
    PlayerType player;
    int Base;
};

char ProcessRRespadj(char* str, struct RRespadjType* prrespadj);

#endif /* _EVRRESPADJ_H_ */
