#include "evWeights.h"
#include "evGame.h"
#include "evFile.h"
#include <iostream>
#include "evRecord.h"
#include "evContrib.h"

int main (int argc, char* argv[])
{
#if 0
    evGame game;
    evFile myf(argv[1]);
    evRecordType evt;
    while ((evt = myf.Next()) != evrtNone)
    {
        if (evt == evrtId)
        {
            std::cout << game << std::endl;
            game.Init();
        }
        switch (evt)
        {
            case evrtId     : game.SetId(myf.FetchId());           break;
            case evrtPlay   : game.SetPlay(myf.FetchPlay());       break;/*std::cout << myf.FetchPlay()    << std::endl;*/ break;
            case evrtCom    : /*std::cout << myf.FetchCom()     << std::endl;*/ break;
            case evrtData   : /*std::cout << myf.FetchData()    << std::endl;*/ break;
            case evrtInfo   : /*std::cout << myf.FetchInfo()    << std::endl;*/ break;
            case evrtRadj   : game.SetRunner(myf.FetchRadj());     break;
            case evrtStart  : game.SetStart(myf.FetchStart());     break;
            case evrtSub    : game.SetStart(myf.FetchStart());     break;
            case evrtVersion: game.SetVersion(myf.FetchVersion()); break;
            default: break;
        }
    }
    std::cout << game << std::endl;
#endif
#if 0
    evWeightDist aDist;
    std::cout << std::hex << (int) evWeight_Batter(aDist) << std::endl;
    evWeightType w;
    w.pos = Pitcher;
    w.weight = 0x3F;
    aDist.push_back(w);
    std::cout << std::hex << (int) evWeight_Batter(aDist) << std::endl;
    evRecordPlay Play("1,0,sprig001,32,SBFBBX,7/F7LD");
    std::cout << Play << std::endl;
#endif
    struct evCArray evC[7] = {0};
    /* 0 On 0 Out E3 : s0 = 0x0400 */
    double Vals[0xF801] = {0.};
    double Out[NumContributors] = {0.};
    double d;
    Vals[0x0400] = 0.4;
    Vals[0x0800] = 0.6;
    Vals[0x4000] = 0.2;
    Vals[0x4400] = 0.2;
    Vals[0x8000] = 0.3;
    Vals[0x8400] = 0.3;
    Vals[0xBC00] = 2.1;
    Vals[0xC000] = 0.0;
    Vals[0xF800] = 0.0;
#if 0
    evC[0].State = 0x4000;
    evC[0].Contrib[Batter] = 0.33;
    evC[0].Contrib[Pitcher] = 0.33;
    evC[0].Contrib[InFielder1] = 0.34;
    evC[1].State = 0x0800;
    evC[1].Contrib[Batter] = 0.;
    evC[1].Contrib[Pitcher] = 0.;
    evC[1].Contrib[InFielder1] = 1.;
    d = evC_Compress(Vals,
                     2,
                     evC,
                     0x400,
                     Out);
    std::cout << d << ',';
    for (int i=0; i<NumContributors; i++) std::cout << Out[i] << ',';
    std::cout << std::endl;
    for (int i=0; i<NumContributors; i++) std::cout << d*Out[i] << ',';
    std::cout << std::endl;
    std::cout << std::endl;

    /* 0 On 0 Out 1B : s0 = 0x0400 */
    evC[0].State = 0x0800;
    evC[0].Contrib[Batter] = 0.5;
    evC[0].Contrib[Pitcher] = 0.5;
    evC[0].Contrib[InFielder1] = 0.;
    d = evC_Compress(Vals,
                     1,
                     evC,
                     0x400,
                     Out);
    std::cout << d << ',';
    for (int i=0; i<NumContributors; i++) std::cout << Out[i] << ',';
    std::cout << std::endl;
    for (int i=0; i<NumContributors; i++) std::cout << d*Out[i] << ',';
    std::cout << std::endl;
    std::cout << std::endl;

    /* 3 On 2 Out HR-robbed : s0 = 0xBC00 */
    evC[0].State = 0x8000;
    evC[0].Contrib[Batter] = 0.5;
    evC[0].Contrib[Pitcher] = 0.5;
    evC[1].State = 0xF800;
    evC[1].Contrib[OutFielderLeft] = 1.;
    evC[1].Contrib[Pitcher] = 0.;
    evC[1].Contrib[InFielder1] = 0.;
    d = evC_Compress(Vals,
                     2,
                     evC,
                     0xBC00,
                     Out);
    std::cout << d << ',';
    for (int i=0; i<NumContributors; i++) std::cout << Out[i] << ',';
    std::cout << std::endl;
    for (int i=0; i<NumContributors; i++) std::cout << d*Out[i] << ',';
    std::cout << std::endl;
#endif
    std::ifstream f;
    f.open("3Ks.bin", std::ios_base::in);
    char indat[44];
    char num;
    f.read(&num, 1);
    for (int i=0; i<num; i++)
    {
        f.read(indat, 44);
        evC[i].State = indat[0] + (indat[1]<<8);
        unsigned int sum = 0x10000;
        for (int j=0; j<NumContributors-1; j++)
        {
            unsigned short c = indat[2+2*j] + (indat[3+2*j]<<8);
            sum -= c;
            evC[i].Contrib[j] = ((double) c)/65536.;
        }
        evC[i].Contrib[Null] = ((double) sum)/65536.;
    }
    f.close();
    d = evC_Compress(Vals,
                     num,
                     evC,
                     0x0000,
                     Out);
    std::cout << d << ',';
    for (int i=0; i<NumContributors; i++) std::cout << Out[i] << ',';
    std::cout << std::endl;
    for (int i=0; i<NumContributors; i++) std::cout << d*Out[i] << ',';
    std::cout << std::endl;
    return 0;
}