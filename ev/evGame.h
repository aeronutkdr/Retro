#ifndef EVGAME_H
#define EVGAME_H
#include "evTypes.h"
#include "evRecordId.h"
#include "evRecordPlay.h"
#include "evRecordRadj.h"
#include "evRecordStart.h"
#include "evRecordVersion.h"
#include <ostream>

#define ARRSIZE (13)
class evGame
{
protected:
    std::string Lineups[2][ARRSIZE];
    std::string Positions[2][ARRSIZE];
    std::string Runners[4];
    //std::vector<>char* Members[NumContributors];
    std::string Id;
    int version;
    int inning;
    int bottom;
    int out; 
public:
    //char* Assign(enum ContributorPosition p, char* m);
    //char* GetContributorAt (enum ContributorPosition p);
    void Init(void);
    std::string SetId(evRecordId id);
    int SetVersion(evRecordVersion v);
    std::string SetStart(evRecordStart s);
    std::string SetRunner(evRecordRadj r);
    void SetPlay(evRecordPlay p);
    evGame(void);
    friend std::ostream& operator<<(std::ostream& os, const evGame& ev);
};

#endif /* EVGAME_H */