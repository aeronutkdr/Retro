#include <bits/stdc++.h>
#include <random>
/* 4 baserunners, 9 fielders, 6 umpires, 2 managers */
enum ContributorPosition
{
    Batter = 0, /* do not use this - it will be 0x10000 - (sum of others) */
    Runner1,
    Runner2,
    Runner3,
    Pitcher,
    Catcher,
    InFielder1,
    InFielder2,
    InFielder3,
    InFielderShort,
    OutFielderLeft,
    OutFielderCenter,
    OutFielderRight,
    UmpireHome,
    Umpire1,
    Umpire2,
    Umpire3,
    UmpireLeft,
    UmpireRight,
    ManagerOffense,
    ManagerDefense,
    NumContributors
};
struct ContributorEntry
{
    enum ContributorPosition contributor;
    unsigned short           weight;
};
struct ProgressionEntry
{
    unsigned short                       ToState;
    std::vector<struct ContributorEntry> Contribs;
};
int main (int argc, char* argv[])
{
    std::minstd_rand0 g;
    std::vector<struct ProgressionEntry> Progression;
    Progression.clear();
    for (unsigned char ProgressionSize = g()&0xF;
         ProgressionSize;
         ProgressionSize--)
    {
        struct ProgressionEntry e;
        e.ToState = g() & 0xFFFF;
        unsigned char Numcontrib = g() % NumContributors;
        unsigned int sum = 0;
        unsigned char C[Numcontrib];
        for (int i = 0; i<Numcontrib; i++)
        {
            C[i] = g() & 0xF;
            sum += C[i];
        }
        unsigned int avail = 0x10000;
        struct ContributorEntry ce;
        for (int i=1; i<Numcontrib; i++)
        {
            ce.contributor = (enum ContributorPosition) i;
            ce.weight = C[i] * 0x10000 / sum;
            avail -= ce.weight;
            if (ce.weight) e.Contribs.push_back(ce);
        }
        ce.contributor = Batter;
        ce.weight = avail;
        if (ce.weight) e.Contribs.push_back(ce);
        Progression.push_back(e);
    }
    for (auto p: Progression)
    {
        std::cout << std::hex << p.ToState << std::endl;
        for (auto c: p.Contribs)
        {
            std::cout << std::dec << c.contributor << " , " << std::hex << c.weight << std::endl;
        }
    }
    return 0;
}