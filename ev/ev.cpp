#include "evWeights.h"
#include "evGame.h"
#include "evFile.h"
#include <iostream>
#include "evRecord.h"

int main (int argc, char* argv[])
{
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
    return 0;
}