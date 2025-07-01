#include "evRRespadj.h"
#include <stdio.h>

char ProcessRRespadj(char* str, struct RRespadjType* prrespadj)
{
    sscanf ("%s,%d", prrespadj->player, &prrespadj->Base);
    return 2;
}

