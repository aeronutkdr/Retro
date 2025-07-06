#ifndef EVCONTRIB_H
#define EVCONTRIB_H
#include "evTypes.h"

struct evCArray
{
    unsigned short State;
    double Contrib[NumContributors];
};

double evC_Compress(double* Vals,
                    int numC,
                    struct evCArray* evC,
                    unsigned short s0,
                    double *Out);

#endif /* EVCONTRIB_H */