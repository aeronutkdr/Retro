#include "evGame.h"
#include <cassert>
#include <iostream>
#include <iomanip>

evGame::evGame(void)
{
    Init();
}

void evGame::Init(void)
{
    version = 0;
    inning = 0;
    bottom = 0;
    out    = 0;
    Id.clear();
    for (int i=0; i<4; i++) Runners[i].clear();
    for (int i=0; i<2; i++)
    {
        for (int j=0; j<ARRSIZE; j++) Lineups[i][j].clear();
        for (int j=0; j<ARRSIZE; j++) Positions[i][j].clear();
    }
    //for (int i=0; i<NumContributors; i++) Members[i] = 0;
}
#if 0
char* evGame::Assign(enum ContributorPosition p, char* m)
{
    char* retval = GetContributorAt(p);
    Members[p] = m;
    return retval;
}
char* evGame::GetContributorAt (enum ContributorPosition p)
{
    return Members[p];
}
#endif

std::string evGame::SetId(evRecordId id)
{
    std::string retval = Id;
    Id = id.getName();
    return retval;
}

int evGame::SetVersion(evRecordVersion v)
{
    int retval = version;
    version = v.getVersion();
    return retval;
}

#if 0
void OutputIf(std::string u, std::string v)
{
    if (u !=v)
    {
        std::cout << u << "!=" << v << std::endl;
    }
}
#else
#define OutputIf(u,v) (((u)==(v)) || (std::cout << (u) << "!=" << (v) << std::endl))
#endif
std::string evGame::SetStart(evRecordStart s)
{
    assert (s.getOrder() < ARRSIZE);
    assert (s.getPos() < ARRSIZE);
    assert (s.getHome() < 2);
    std::string retval1 = Lineups[s.getHome()][s.getOrder()];
    int pos = s.getPos();
    if (pos > 10)
    {
        for (int i=0; i<11; i++)
        {
            if (retval1 == this->Positions[s.getHome()][i])
            {
                pos = i;
                break;
            }
        }
    }
    std::string retval2 = Positions[s.getHome()][pos];
    OutputIf(retval1,retval2);
    //assert (retval1 == retval2);
    Lineups[s.getHome()][s.getOrder()] = s.getName();
    Positions[s.getHome()][pos] = s.getName();
    return retval1;
}

void evGame::SetPlay(evRecordPlay p)
{
    //std::cout << p << std::endl;
    //assert(this->inning == p.getInning());
    //assert(this->bottom == p.getBottom());
    this->Runners[0] = p.getName();
    std::string ev = p.getEvent();
    unsigned short st = 0;
    for (int i=4; i>0; i--)
    {
       st <<= 1;
       st |= Runners[i-1].empty()?0:1; 
    }
    st <<= 10;
    st |= (out << 14);
    unsigned short res = p.Process(st);
    for (int i=4; i>0; i--)
    {
        std::string s = Runners[i-1];
        int to = res>>((i-1)<<1);
        if (to)
        {
            Runners[to] = s;
        }
    }
    out = st>>8;
}

std::string evGame::SetRunner(evRecordRadj r)
{
    assert (r.getBase() < 4);
    std::string retval = Runners[r.getBase()];
    Runners[r.getBase()] = r.getName();
    return retval;
}

std::ostream& operator<<(std::ostream& os, const evGame& ev)
{
    os << "Game/"
       << ev.Id     << '/'
       << ev.inning << '/'
       << ev.bottom << '/'
       << ev.out;
    for (int i=0; i<4; i++) os << '/' << ev.Runners[i];
    os << std::endl;
    for (int i=0; i<2; i++)
    {
        os << "i=" << i << " Lineup" << std::endl;
        for (int j=0; j<ARRSIZE; j++) os << std::setw(2) << j << ": " << ev.Lineups[i][j] << std::endl;
        os << "i=" << i << " Position" << std::endl;
        for (int j=0; j<ARRSIZE; j++) os << std::setw(2) << j << ": " << ev.Positions[i][j] << std::endl;
    }
    return os;
}