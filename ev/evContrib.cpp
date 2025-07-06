#include "evContrib.h"
#include <iostream>

static double evC_StateValue(double* Vals, unsigned short s)
{
    return Vals[s] - (((double)((s>>10)&1)) +
                      ((double)((s>>11)&1)) +
                      ((double)((s>>12)&1)) +
                      ((double)((s>>13)&1)) +
                      ((double)((s>>14)&3)));
}

void Dump (unsigned short s0)
{
    std::cout << (s0>>14) << " out " 
              << ((s0&0x2000)?"3":" ")
              << ((s0&0x1000)?"2":" ")
              << ((s0&0x0800)?"1":" ")
              << ((s0&0x0400)?"H":" ")
              << std::endl;
}
double evC_Compress(double* Vals,
                    int numC,
                    struct evCArray* evC,
                    unsigned short s0,
                    double *Out)
{
    for (int i=0; i<NumContributors; i++)
    {
        Out[i] = 0.;
    }
    double v0 = evC_StateValue(Vals, s0);
    double vi = v0;
    Dump(s0);
    for (int i=0; i<numC; i++)
    {
        const struct evCArray* e = evC + i;
        double v = evC_StateValue(Vals, e->State);
        Dump(e->State);
        std::cout << std::hex << s0 << " to " << e->State << "= " << std::dec << (v-vi) << std::endl;
        for(int j=0; j<NumContributors; j++)
        {
            Out[j] += (v - vi) * e->Contrib[j];
        }
        vi = v;
        s0 = e->State;
    }
    for(int j=0; j<NumContributors; j++)
    {
        Out[j] /= (vi - v0);
    }
    return (vi-v0);
}