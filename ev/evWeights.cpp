#include "evWeights.h"

unsigned char evWeight_Batter(evWeightDist& wd)
{
    unsigned char retval = 0xFF;
    for (auto w: wd)
    {
        retval -= w.weight;
    }
    return retval;
}