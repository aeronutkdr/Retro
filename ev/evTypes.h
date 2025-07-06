#ifndef EVTYPES_H
#define EVTYPES_H

/* 4 baserunners, 9 fielders, 6 umpires, 2 managers */
enum ContributorPosition
{
    Batter = 0,
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
    Null,
    NumContributors
};

#endif /* EVTYPES_H */