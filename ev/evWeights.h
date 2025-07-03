#ifndef EVWEIGHTS_H
#define EVWEIGHTS_H
#include "evTypes.h"
#include <vector>

class evWeightType
{
public:
    enum ContributorPosition pos;
    unsigned char            weight;
};

/* total = 0xFF */
typedef std::vector<evWeightType> evWeightDist;

unsigned char evWeight_Batter(evWeightDist& wd);

#endif /* EVWEIGHTS_H */