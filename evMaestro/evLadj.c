#include "evLadj.h"
#include <stdio.h>

char ProcessLadj(char* str, struct LadjType* pladj)
{
    sscanf (str, "%d,%d", &pladj->Home, &pladj->Position);
    return 2;
}