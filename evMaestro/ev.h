#ifndef _EV_H_
#define _EV_H_

struct evID
{
    char* HTDateGame;
};
struct evVersion
{
    char* Value;
};
struct evInfo
{
    char* Label;
    char* Data;
};
struct evStartSub
{
    char* ID;
    char* Name;
    char* IsHome;
    char* BatOrder;
    char* FieldPos;
};
struct evPlay
{
    char* Inning;
    char* IsHome;
    char* ID;
    char* Count;
    char* Pitches;
    char* Event;
};
struct evBPAdj /* Batting or Pitching */
{
    char* ID;
    char* Hand;
};
struct evLRAdj /* Lineup/Runner */
{
    char* IsHome;
    char* Pos;
};
struct evPRAdj /* PitcherResponsibility */
{
    char* ID;
    char* Occ;
};
struct evData
{
    char* Label;
    char* ID;
    char* Value;
};
struct evCom
{
    char* Value;
};

enum evEnum
{
    evBAdj,
    evCom,
    evData,
    evID,
    evInfo,
    evLAdj,
    evPAdj,
    evPRAdj,
    evPlay,
    evRAdj,
    evStart,
    evSub,
    evVersion,
    evUNK
};

struct ev
{
    enum evEnum type;
    union 
    {
        char*             ptrs[6];
        struct evBPAdj    BPAdj;    /* Batting/Pitching       */
        struct evCom      Com;      /* Comment                */
        struct evData     Data;     /* Data                   */
        struct evID       ID;       /* GameID                 */
        struct evInfo     Info;     /* Game Information       */
        struct evLRAdj    LRAdj;    /* Lineup/Runner          */
        struct evPRAdj    PRAdj;    /* Pitcher Responsibility */
        struct evPlay     Play;     /* Game Play              */
        struct evStartSub StartSub; /* Start/Substitution     */
        struct evVersion  Version;  /* Version                */
    } data;
};

void evCreate(char* str, struct ev *pev);
void evProcess(struct ev *pev);
void evDump(struct ev *pev);

#define NAME_SIZE (40)
#define ID_SIZE (8)
#define TEAM_SIZE (4)
#define POS_SIZE (2)
struct evRoster
{
    char ID[ID_SIZE+1];
    char LastName[NAME_SIZE+1];
    char FirstName[NAME_SIZE+1];
    char Bat;
    char Throw;
    char Team[TEAM_SIZE+1];
    char Position[POS_SIZE+1];
};

void evRosterAdd(char* s);
char evRosterBats(char* id);
char evRosterThrows(char* id);
void evRosterDump(void);

#endif /* _EV_H_ */
