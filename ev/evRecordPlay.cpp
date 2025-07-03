#include "evRecordPlay.h"
#include <cassert>
#include <iostream>

evRecordPlay::evRecordPlay(std::string s)
{
    std::size_t r = 0;
    std::size_t t = s.find_first_of(',');
    int i;
    for (i=0; t != s.npos; i++, r = t+1, t = s.find_first_of(',', r))
    {
        switch (i)
        {
            case 0:
                assert ((t-r) > 0);
                inning  = std::stoi(s.substr(r,t-r)); break;
            case 1:
                assert ((t-r) == 1);
                bottom  = std::stoi(s.substr(r,t-r)); break;
            case 2:
                assert ((t-r) == 8);
                name    = s.substr(r,t-r);            break;
            case 3:
                assert ((t-r) == 2);
                balls   = std::stoi(s.substr(r,1));
                strikes = std::stoi(s.substr(r+1,1)); break;
            case 4:
                pitches = s.substr(r,t-r);            break;
            default: break;
        }
    }
    assert (i==5);
    event = s.substr(r);
}

std::ostream& operator<<(std::ostream& os, const evRecordPlay& ev)
{
    os << "Play/"
       << ev.inning  << '/'
       << ev.bottom  << '/'
       << ev.name    << '/'
       << ev.balls   << '/'
       << ev.strikes << '/'
       << ev.pitches << '/'
       << ev.event;
    return os;
}

unsigned short evRecordPlay::ProcessEvent(std::string ev)
{
    unsigned short retval = 0;
    /* 333322221111HHHH */
    std::string p1 = ev.substr(ev.find_first_of('/'));
    if (p1 == "K") { retval = 0xF; }
    if (p1[0] > '1' && p1[0] <= '9') {  retval = 0x321F ;}
    if (p1[0] == "NP") {retval = 0x3210;}
    if (p1[0] == 'S') {retval = 0x3211;}
    if (p1 == "HR") {retval = 0x4444;}
play,1,1,renfh001,10,BX,HR/F7LD.1-H
play,2,0,espis001,02,FTFFB,WP.1-2
play,2,1,ohopl001,32,BCSBBX,64(1)3/GDP/G6
play,3,1,ohtas001,21,BFBX,HR/F78XD.1-H
play,3,1,renda001,32,BFBSBB,W
play,4,0,chapm001,32,BBBCFB,W
play,4,0,varsd001,11,BCX,13/G1S-
play,4,1,ohopl001,10,BX,HR/F7D
play,5,0,espis001,12,CFBX,54(1)/FO/G56S.B-1
play,5,0,kierk001,01,SX,43/G34
play,5,1,drurb001,00,X,53/G5
play,6,0,sprig001,32,FFBBBFB,W
play,6,0,guerv002,01,*SH,HP.2-3;1-2
play,6,0,chapm001,00,X,HR/F89XD.3-H;2-H;1-H
play,6,0,merrw001,21,.BFB>X,E1/G6MS-.1-2;B-1
play,6,0,kierk001,21,BBSX,T9/L9L+.3-H(UR);1-H(UR)
play,7,0,bichb001,11,.FBH,HP
play,7,0,kirka001,30,.VVVV,IW
play,7,1,ohtas001,32,CBBBCX,43/G34
play,8,0,bichb001,31,.BBCBX,53/G5
play,8,0,guerv002,01,TX,13/G1S
play,8,0,chapm001,12,BFFX,13/G1S
play,8,1,drurb001,10,BX,HR/F8XD
play,9,0,varsd001,32,FBFBBB,W
play,9,0,merrw001,11,CBX,54(1)/FO/G5.B-1
play,9,0,biggc002,00,1,PO1(13)
play,9,1,wardt002,32,FBBSBB,W.1-2
play,9,1,ohtas001,30,BBBB,W.2-3;1-2
play,9,1,renda001,22,BCBFH,HP.3-H;2-3;1-2
play,9,1,renfh001,01,.CX,D7/G5+.3-H;2-H;1-3
play,10,0,kirka001,32,....FBBFFBX,63/G6.2-3
play,10,0,kierk001,01,SX,DGR/F9LD.3-H(UR)
play,10,1,troum001,32,FBBBF>B,W.3-H(UR);2-3;1-2
play,10,1,ohtas001,02,.CSX,43/G34


}
#define chToBase(c) (((c)=='H')?0:((c)-'0'))
/* OO332211HH */                     /* OO321HBBBFFFFFSS */
/* 9876543210 */                     /* FEDCBA9876543210 */
unsigned short evRecordPlay::Process(unsigned short state)
{
    int Outs = state >> 14;
    int R3 = (state >> 13) & 1;
    int R2 = (state >> 12) & 1;
    int R1 = (state >> 11) & 1;
    int H  = (state >> 10) & 1;
    //int p2Start = this->event.find_first_of('/');
    unsigned short retval = (R3?(3<<6):0) |
                            (R2?(2<<4):0) |
                            (R1?(1<<2):0) |
                            (H?(0<<0):0);
    for (std::size_t p3Start = this->event.find_first_of('.');
         p3Start != std::string::npos;
         p3Start = this->event.substr(p3Start).find_first_of(';'))
    {
        int startBase = chToBase(event[p3Start]);
        int endBase = chToBase(event[p3Start+2]);
        retval &= ~(3 << (startBase<<1));
        if (event[p3Start+1] == '-')
        {
            retval |= endBase << (startBase<<1);
        }
        else if (event[p3Start+1] == 'X')
        {
            Outs++;
        }
    }
    retval |= (Outs << 14);
    return retval;
    /*
    The first part of an event is a description of the basic play.

    The second part is a modifier for the first part and is separated from
     it with a forward slash, "/". In fact, there may be more than one modifier.
     A typical use of modifiers is to specify hit locations.
     For example, "D8/78" indicates a double fielded by the center fielder on a
     ball hit to left center. A complete list of modifiers excepting hit
     locations is given below. When more than one modifier is used, each is
     introduced by a "/".

    The third part describes the advance of any runners, separated from the
     earlier parts by a period. A successful advance is indicated by a dash, "-".
     An out made while advancing is indicated by an X. 2-3 indicates a runner has
     advanced from second to third on the play. 1X2 indicates the runner was out
     at second advancing from first. If a base runner is not listed as advancing
     he remains on the base he was on. In some cases lack of advance is indicated
     explicitly by an advance starting and ending on the same base such as 3-3.
     When put outs are made on base runners the advance field indicates fielding
     data and errors if they occur. See below for a complete description for
     advances. Note that any advances after the first are separated by semicolons.
    */
}