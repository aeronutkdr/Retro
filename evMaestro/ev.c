#include "ev.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
/* https://www.retrosheet.org/eventfile.htm */
/* https://www.retrosheet.org/datause.html */

#define ID_STR      "id"
#define VERSION_STR "version"
#define INFO_STR    "info"
#define START_STR   "start"
#define PLAY_STR    "play"
#define SUB_STR     "sub"
#define DATA_STR    "data"
#define COM_STR     "com"
#define BADJ_STR    "badj"
#define PADJ_STR    "padj"
#define LADJ_STR    "ladj"
#define RADJ_STR    "radj"
#define PRADJ_STR   "pradj"

void evCreate(char* str, struct ev *Ev)
{
    char** p = Ev->data.ptrs;
    char* s = strchr(str,',');
    while (s && *s)
    {
        *s = 0;
        s++;
        if (*s == '"')
        {
            *p = s+1;
            s = strchr(s+1,'"'); /* s must exist */
            *s = 0;
            s++; /* could result in *s == 0 */
        }
        else
        {
            *p = s;
            s = strchr(s,','); /* could result in s == 0 */
        }
        p++;
    }
    Ev->type = (!strcmp(str, BADJ_STR   ))?evBAdj:
               (!strcmp(str, PADJ_STR   ))?evPAdj:
               (!strcmp(str, COM_STR    ))?evCom:
               (!strcmp(str, DATA_STR   ))?evData:
               (!strcmp(str, ID_STR     ))?evID:
               (!strcmp(str, INFO_STR   ))?evInfo:
               (!strcmp(str, LADJ_STR   ))?evLAdj:
               (!strcmp(str, RADJ_STR   ))?evRAdj:
               (!strcmp(str, PRADJ_STR  ))?evPRAdj:
               (!strcmp(str, PLAY_STR   ))?evPlay:
               (!strcmp(str, START_STR  ))?evStart:
               (!strcmp(str, SUB_STR    ))?evSub:
               (!strcmp(str, VERSION_STR))?evVersion:
                                           evUNK;
}
void evDumpBPAdj (struct evBPAdj *p)
{
    printf ("Hand       -> %s\n", p->Hand);
    printf ("ID         -> %s\n", p->ID);
}
void evDumpCom (struct evCom *p)
{
    printf ("Value      -> %s\n", p->Value);
}
void evDumpData (struct evData *p)
{
    printf ("ID         -> %s\n", p->ID);
    printf ("Label      -> %s\n", p->Label);
    printf ("Value      -> %s\n", p->Value);
}
void evDumpID (struct evID *p)
{
    printf ("HTDateGame -> %s\n", p->HTDateGame);
}
void evDumpInfo (struct evInfo *p)
{
    printf ("Data       -> %s\n", p->Data);
    printf ("Label      -> %s\n", p->Label);
}
void evDumpLAdj (struct evLRAdj *p)
{
    printf ("IsHome     -> %s\n", p->IsHome);
    printf ("Pos        -> %s\n", p->Pos);
}
void evDumpPRAdj (struct evPRAdj *p)
{
    printf ("ID         -> %s\n", p->ID);
    printf ("Occ        -> %s\n", p->Occ);
}
void evDumpPlay (struct evPlay *p)
{
    printf ("Count      -> %s\n", p->Count);
    printf ("Event      -> %s\n", p->Event);
    printf ("ID         -> %s\n", p->ID);
    printf ("Inning     -> %s\n", p->Inning);
    printf ("IsHome     -> %s\n", p->IsHome);
    printf ("Pitches    -> %s\n", p->Pitches);
}
void evDumpLRAdj (struct evLRAdj *p)
{
    printf ("IsHome     -> %s\n", p->IsHome);
    printf ("Pos        -> %s\n", p->Pos);
}
void evDumpStartSub(struct evStartSub *p)
{
    printf ("BatOrder   -> %s\n", p->BatOrder);
    printf ("FieldPos   -> %s\n", p->FieldPos);
    printf ("ID         -> %s\n", p->ID);
    printf ("IsHome     -> %s\n", p->IsHome);
    printf ("Name       -> %s\n", p->Name);
}
void evDumpVersion (struct evVersion *p)
{
    printf ("Value      -> %s\n", p->Value);
}
void evDump(struct ev *pev)
{
    switch (pev->type)
    {
    case evBAdj:    evDumpBPAdj   (&pev->data.BPAdj);    break;
    case evCom:     evDumpCom     (&pev->data.Com);      break;
    case evData:    evDumpData    (&pev->data.Data);     break;
    case evID:      evDumpID      (&pev->data.ID);       break;
    case evInfo:    evDumpInfo    (&pev->data.Info);     break;
    case evLAdj:    evDumpLAdj    (&pev->data.LRAdj);    break;
    case evPAdj:    evDumpBPAdj   (&pev->data.BPAdj);    break;
    case evPRAdj:   evDumpPRAdj   (&pev->data.PRAdj);    break;
    case evPlay:    evDumpPlay    (&pev->data.Play);     break;
    case evRAdj:    evDumpLRAdj   (&pev->data.LRAdj);    break;
    case evStart:   evDumpStartSub(&pev->data.StartSub); break;
    case evSub:     evDumpStartSub(&pev->data.StartSub); break;
    case evVersion: evDumpVersion (&pev->data.Version);  break;
    case evUNK:     printf ("unknown\n");                break;
    default:        printf ("default\n");                break;
    }
}

enum EventType
{
    evUnknownEvent          =  0,
    evNoEvent               =  1,
    evGenericOut            =  2,
    evStrikeout             =  3,
    evStolenBase            =  4,
    evDefensiveIndifference =  5,
    evCaughtStealing        =  6,
    evPickoffError          =  7,
    evPickoff               =  8,
    evWildPitch             =  9,
    evPassedBall            = 10,
    evBalk                  = 11,
    evOtherAdvance          = 12,
    evFoulError             = 13,
    evWalk                  = 14,
    evIntentionalWalk       = 15,
    evHitByPitch            = 16,
    evInterference          = 17,
    evError                 = 18,
    evFieldersChoice        = 19,
    evSingle                = 20,
    evDouble                = 21,
    evTriple                = 22,
    evHomeRun               = 23,
    evMissingPlay           = 24,
    evUnassist              = 25,
    evAssisted              = 26,
    evLinedDoublePlay       = 27,
    evGroundDoublePlay      = 28
};

/* https://www.retrosheet.org/eventfile.htm#6 */
enum ModifierType
{
  emod_THToB,/* throw to base % */
  emod_BGDP, /* bunt grounded into double play */
  emod_BINT, /* batter interference */
  emod_BOOT, /* batting out of turn */
  emod_BPDP, /* bunt popped into double play */
  emod_COUB, /* courtesy batter */
  emod_COUF, /* courtesy fielder */
  emod_COUR, /* courtesy runner */
  emod_FINT, /* fan interference */
  emod_IPHR, /* inside the park home run */
  emod_MREV, /* manager challenge of call on the field */
  emod_PASS, /* a runner passed another runner and was called out */
  emod_RINT, /* runner interference */
  emod_UINT, /* umpire interference */
  emod_UREV, /* umpire review of call on the field */
  emod_FDP,  /* fly ball double play */
  emod_GDP,  /* ground ball double play */
  emod_GTP,  /* ground ball triple play */
  emod_INT,  /* interference */
  emod_LDP,  /* lined into double play */
  emod_LTP,  /* lined into triple play */
  emod_NDP,  /* no double play credited for this play */
  emod_OBS,  /* obstruction (fielder obstructing a runner) */
  emod_AP,   /* appeal play */
  emod_BG,   /* ground ball bunt */
  emod_BL,   /* line drive bunt */
  emod_BP,   /* bunt pop up */
  emod_BR,   /* runner hit by batted ball */
  emod_DP,   /* unspecified double play */
  emod_FL,   /* foul */
  emod_FO,   /* force out */
  emod_IF,   /* infield fly rule */
  emod_SF,   /* sacrifice fly */
  emod_SH,   /* sacrifice hit (bunt) */
  emod_TH,   /* throw */
  emod_TP,   /* unspecified triple play */
  emod_C,    /* called third strike */
  emod_E,    /* error on $ */
  emod_F,    /* fly */
  emod_G,    /* ground ball */
  emod_L,    /* line drive */
  emod_P,    /* pop fly */
  emod_R,    /* relay throw from the initial fielder to $ with no out made */
  emod_NUM
};
static const char* ModifierFilters[] =
{
  [emod_AP]   = "AP",
  [emod_BG]   = "BG",
  [emod_BGDP] = "BGDP",
  [emod_BINT] = "BINT",
  [emod_BL]   = "BL",
  [emod_BOOT] = "BOOT",
  [emod_BP]   = "BP",
  [emod_BPDP] = "BPDP",
  [emod_BR]   = "BR",
  [emod_C]    = "C",
  [emod_COUB] = "COUB",
  [emod_COUF] = "COUF",
  [emod_COUR] = "COUR",
  [emod_DP]   = "DP",
  [emod_E]    = "E",
  [emod_F]    = "F",
  [emod_FDP]  = "FDP",
  [emod_FINT] = "FINT",
  [emod_FL]   = "FL",
  [emod_FO]   = "FO",
  [emod_G]    = "G",
  [emod_GDP]  = "GDP",
  [emod_GTP]  = "GTP",
  [emod_IF]   = "IF",
  [emod_INT]  = "INT",
  [emod_IPHR] = "IPHR",
  [emod_L]    = "L",
  [emod_LDP]  = "LDP",
  [emod_LTP]  = "LTP",
  [emod_MREV] = "MREV",
  [emod_NDP]  = "NDP",
  [emod_OBS]  = "OBS",
  [emod_P]    = "P",
  [emod_PASS] = "PASS",
  [emod_R]    = "R",
  [emod_RINT] = "RINT",
  [emod_SF]   = "SF",
  [emod_SH]   = "SH",
  [emod_TH]   = "TH",
  [emod_THToB]= "THToB",
  [emod_TP]   = "TP",
  [emod_UINT] = "UINT",
  [emod_UREV] = "UREV",
};

#define NUM_MODS (4)
struct ModifierListType
{
    unsigned char mNum;
    struct
    {
        enum ModifierType mModifier;
        unsigned char     mAssignee;
    } mMods[NUM_MODS];
};

void evProcessModifiers(struct ModifierListType *m, char* s)
{
    m->mNum = 0;
    while (s)
    {
        assert(m->mNum < NUM_MODS);
        char* found = 0;
        enum ModifierType me;
        for (me = 0; me < emod_NUM && !found; me++)
        {
            found = (strstr(s, ModifierFilters[me]));
        }
        assert (found);
        m->mMods[m->mNum].mModifier = me;
        switch (me)
        {
            case emod_THToB:/* throw to base % */
            case emod_E:    /* error on $ */
            case emod_R:    /* relay throw from the initial fielder to $ with no out made */
                m->mMods[m->mNum].mAssignee = s[strlen(ModifierFilters[me])];
            break;
        }
        m->mNum++;
        s=strchr(s, '/');
    }
}

#define NUM_ADVANCES (4)
struct AdvanceListType
{
    unsigned char mNum;
    struct _Adv
    {
        unsigned char mFrom;
        unsigned char mTo;
    } mAdvs[NUM_ADVANCES];
};

void evProcessAdvances(struct AdvanceListType *a, char* s)
{
    a->mNum = 0;
    memset (a->mAdvs,0xFF,sizeof(a->mAdvs));
    struct _Adv* t = a->mAdvs;
    while (s)
    {
        assert(a->mNum < NUM_MODS);
        if (*s != ';')
        {
            if (t->mFrom == 0xFF)
                t->mFrom = *s;
            else if (*s != '-')
            {
                t->mTo = *s;
                a->mNum++;
                t++;
            }
        }
        s++;
    }
}

struct EventDataType
{
    enum EventType          mEv;
    struct ModifierListType mList;
};

const static struct evEventResultsType
{
    unsigned char BatterEventFlag;
    unsigned char OutsOnPlay;
    unsigned char ForcedAdvance;
    unsigned char EventType;
    unsigned char AB;
} evEventResults[] =
 {
                              /* BatterEventFlag OutsOnPlay ForcedAdvance EventType AB*/
    [evUnknownEvent]          = {      1        ,     0    ,       0     ,     0,    0},
    [evNoEvent]               = {      0        ,     0    ,       0     ,     1,    0},
    [evGenericOut]            = {      1        ,     1    ,       0     ,     2,    1},
    [evStrikeout]             = {      1        ,     1    ,       0     ,     3,    1},
    [evStolenBase]            = {      0        ,     0    ,       0     ,     4,    0},
    [evDefensiveIndifference] = {      0        ,     0    ,       0     ,     5,    0},
    [evCaughtStealing]        = {      0        ,     1    ,       0     ,     6,    0},
    [evPickoffError]          = {      0        ,     0    ,       0     ,     7,    0},
    [evPickoff]               = {      0        ,     1    ,       0     ,     8,    0},
    [evWildPitch]             = {      0        ,     0    ,       0     ,     9,    0},
    [evPassedBall]            = {      0        ,     0    ,       0     ,    10,    0},
    [evBalk]                  = {      0        ,     0    ,       0     ,    11,    0},
    [evOtherAdvance]          = {      0        ,     0    ,       1     ,    12,    0},
    [evFoulError]             = {      0        ,     0    ,       0     ,    13,    0},
    [evWalk]                  = {      1        ,     0    ,       1     ,    14,    0},
    [evIntentionalWalk]       = {      1        ,     0    ,       1     ,    15,    0},
    [evHitByPitch]            = {      1        ,     0    ,       1     ,    16,    0},
    [evInterference]          = {      1        ,     0    ,       1     ,    17,    0},
    [evError]                 = {      1        ,     0    ,       1     ,    18,    1},
    [evFieldersChoice]        = {      1        ,     1    ,       0     ,    19,    0},
    [evSingle]                = {      1        ,     0    ,       1     ,    20,    1},
    [evDouble]                = {      1        ,     0    ,       2     ,    21,    1},
    [evTriple]                = {      1        ,     0    ,       3     ,    22,    1},
    [evHomeRun]               = {      1        ,     0    ,       4     ,    23,    1},
    [evMissingPlay]           = {      1        ,     0    ,       0     ,    24,    0},
    [evUnassist]              = {      1        ,     1    ,       0     ,     2,    1},
    [evAssisted]              = {      1        ,     1    ,       0     ,     2,    1},
    [evLinedDoublePlay]       = {      1        ,     2    ,       0     ,    27,    1},
    [evGroundDoublePlay]      = {      1        ,     2    ,       0     ,    28,    1}
};

#define GAME_ID_SIZE (12)
#define PLAYER_ID_SIZE (8)
#define TEAM_ID_SIZE (3)
struct ErrorEntry
{
    unsigned char Player;
    unsigned char Type;
};
static struct ProcessData
{
    char ID     [GAME_ID_SIZE+1];
    struct InfoType
    {
        char visteam[TEAM_ID_SIZE+1];
    } info;
    char Players[50][PLAYER_ID_SIZE+1];
    char* Lineups[2][9];
    char* Defense[2][10];
    char* Bases[4];
    unsigned char Score[2];
    unsigned char NumPlayers;
    unsigned char Inning;
    unsigned char Bottom;
    unsigned char Balls;
    unsigned char Strikes;
    unsigned char Outs;
    char* Pitches;
    char* EventText;
    char isLeadoff;
    char isPinchHit;
    enum EventType Event;
    char batterEventFlag;
    char ABFlag;
    char SHFlag;
    char SFFlag;
    unsigned char OutsOnPlay;
    char DoublePlayFlag;
    char TriplePlayFlag;
    unsigned char RBI;
    char WildPitchFlag;
    char PassedBallFlag;
    char FieldedBy;
    char BattedBall;
    char BuntFlag;
    char FoulFlag;
    char HitLocation[5];
    unsigned char NumErrors;
    struct ErrorEntry Error[3];
    unsigned char Dest[4];
    char PlayOn[4][10]; /* ? */
    char SB[3];
    char CS[3];
    char PO[3];
    char* Resp[3];
    char NewGameFlag;
    char EndGameFlag;
    unsigned char Pinch[3];
    char Removed[4][10]; /* ?? */
    unsigned char RemovedBatPos;
    unsigned char Putout[3];
    unsigned char Assist[5];
    unsigned char EventNum;
} Process;

void evProcessBPAdj (struct evBPAdj *p)
{
    /*
    printf ("Hand       -> %s\n", p->Hand);
    printf ("ID         -> %s\n", p->ID);
    */
}
void evProcessCom (struct evCom *p)
{
    /*
    printf ("Value      -> %s\n", p->Value);
    */
}
void evProcessData (struct evData *p)
{
    /*
    printf ("ID         -> %s\n", p->ID);
    printf ("Label      -> %s\n", p->Label);
    printf ("Value      -> %s\n", p->Value);
    */
}
void evProcessID (struct evID *p)
{
    memset (&Process, 0, sizeof (struct ProcessData));
    strcpy (Process.ID, p->HTDateGame);
    Process.isLeadoff = 1;
    Process.NewGameFlag = 1;
}
void evProcessInfo (struct evInfo *p)
{
         if (!strcmp(p->Label, "visteam")) strcpy(Process.info.visteam, p->Data);
    else if (!strcmp(p->Label, "hometeam"));
    else if (!strcmp(p->Label, "date"));
    else if (!strcmp(p->Label, "site"));
    else if (!strcmp(p->Label, "number"));
    else if (!strcmp(p->Label, "starttime"));
    else if (!strcmp(p->Label, "daynight"));
    else if (!strcmp(p->Label, "usedh"));
    else if (!strcmp(p->Label, "umphome"));
    else if (!strcmp(p->Label, "ump1b"));
    else if (!strcmp(p->Label, "ump2b"));
    else if (!strcmp(p->Label, "ump3b"));
    else if (!strcmp(p->Label, "pitches"));
    else if (!strcmp(p->Label, "oscorer"));
    else if (!strcmp(p->Label, "temp"));
    else if (!strcmp(p->Label, "winddir"));
    else if (!strcmp(p->Label, "windspeed"));
    else if (!strcmp(p->Label, "fieldcond"));
    else if (!strcmp(p->Label, "precip"));
    else if (!strcmp(p->Label, "sky"));
    else if (!strcmp(p->Label, "timeofgame"));
    else if (!strcmp(p->Label, "attendance"));
    else if (!strcmp(p->Label, "wp"));
    else if (!strcmp(p->Label, "lp"));
    else if (!strcmp(p->Label, "save"));
    else assert(0);
}
void evProcessLAdj (struct evLRAdj *p)
{
    /*
    printf ("IsHome     -> %s\n", p->IsHome);
    printf ("Pos        -> %s\n", p->Pos);
    */
}
void evProcessPRAdj (struct evPRAdj *p)
{
    /*
    printf ("ID         -> %s\n", p->ID);
    printf ("Occ        -> %s\n", p->Occ);
    */
}
static char* evFindPlayer(char* ID)
{
    char* found = 0;
    for (int i=0; i<Process.NumPlayers && !found; i++)
    {
        if (!strcmp(ID, Process.Players[i]))
            found = Process.Players[i];
    }
    return found;
}
static int evFindDefense(int home, char* ID)
{
    int found = -1;
    for (int i=0; i<10 && (found<0); i++)
    {
        if (!strcmp(ID, Process.Defense[home][i]))
            found = i;
    }
    return found;
}
static int evFindLineup(int home, char* ID)
{
    int found = -1;
    for (int i=0; i<9 && (found<0); i++)
    {
        if (!strcmp(ID, Process.Lineups[home][i]))
            found = i;
    }
    return found;
}
static enum EventType evParseEventFull(char* s)
{
    char* SlashLoc = strchr(s, '/');

    enum EventType retval = -1;
         if (!strncmp (s, "POCS", 4)) retval = evPickoff;
    else if (!strncmp (s, "FLE" , 3)) retval = evFoulError;
    else if (!strncmp (s, "DGR" , 3)) retval = evDouble;
    else if (!strncmp (s, "PO"  , 2)) retval = evPickoff;
    else if (!strncmp (s, "CS"  , 2)) retval = evCaughtStealing;
    else if (!strncmp (s, "BK"  , 2)) retval = evBalk;
    else if (!strncmp (s, "DI"  , 2)) retval = evDefensiveIndifference;
    else if (!strncmp (s, "OA"  , 2)) retval = evOtherAdvance;
    else if (!strncmp (s, "PB"  , 2)) retval = evPassedBall;
    else if (!strncmp (s, "WP"  , 2)) retval = evWildPitch;
    else if (!strncmp (s, "SB"  , 2)) retval = evStolenBase;
    else if (!strncmp (s, "HP"  , 2)) retval = evHitByPitch;
    else if (!strncmp (s, "NP"  , 2)) retval = evNoEvent;
    else if (!strncmp (s, "FC"  , 2)) retval = evFieldersChoice;
    else if (*s == 'C')               retval = evInterference;
    else if (*s == 'S')               retval = evSingle;
    else if (*s == 'D')               retval = evDouble;
    else if (*s == 'T')               retval = evTriple;
    else if (*s == 'E')               retval = evError;
    else if (*s == 'H')               retval = evHomeRun;
    else if (*s == 'I')               retval = evIntentionalWalk;
    else if (*s == 'W')               retval = evWalk;
    else if (*s == 'K')               retval = evStrikeout;
    else if (strlen(s) == 1)          retval = evUnassist;
    else if (strlen(s) == 2)          retval = evAssisted;
    else if (strchr(s, 'B'))          retval = evLinedDoublePlay;
    else                              retval = evGroundDoublePlay;
    char* PeriodLoc = strchr(s, '.');
    while (SlashLoc)
    {
        char* Next = strchr(SlashLoc+1, '/');
    }
    return retval;
}
static enum EventType evParseEvent(char* s)
{
    enum EventType retval = -1;
         if (!strncmp (s, "POCS", 4)) retval = evPickoff;
    else if (!strncmp (s, "FLE" , 3)) retval = evFoulError;
    else if (!strncmp (s, "DGR" , 3)) retval = evDouble;
    else if (!strncmp (s, "PO"  , 2)) retval = evPickoff;
    else if (!strncmp (s, "CS"  , 2)) retval = evCaughtStealing;
    else if (!strncmp (s, "BK"  , 2)) retval = evBalk;
    else if (!strncmp (s, "DI"  , 2)) retval = evDefensiveIndifference;
    else if (!strncmp (s, "OA"  , 2)) retval = evOtherAdvance;
    else if (!strncmp (s, "PB"  , 2)) retval = evPassedBall;
    else if (!strncmp (s, "WP"  , 2)) retval = evWildPitch;
    else if (!strncmp (s, "SB"  , 2)) retval = evStolenBase;
    else if (!strncmp (s, "HP"  , 2)) retval = evHitByPitch;
    else if (!strncmp (s, "NP"  , 2)) retval = evNoEvent;
    else if (!strncmp (s, "FC"  , 2)) retval = evFieldersChoice;
    else if (*s == 'C')               retval = evInterference;
    else if (*s == 'S')               retval = evSingle;
    else if (*s == 'D')               retval = evDouble;
    else if (*s == 'T')               retval = evTriple;
    else if (*s == 'E')               retval = evError;
    else if (*s == 'H')               retval = evHomeRun;
    else if (*s == 'I')               retval = evIntentionalWalk;
    else if (*s == 'W')               retval = evWalk;
    else if (*s == 'K')               retval = evStrikeout;
    else if (strlen(s) == 1)          retval = evUnassist;
    else if (strlen(s) == 2)          retval = evAssisted;
    else if (strchr(s, 'B'))          retval = evLinedDoublePlay;
    else                              retval = evGroundDoublePlay;
    return retval;
}
#define NZ(s,z) ((s)?(s):(z))
#define isT(c) ((c)=='T')
#define TF(i) ((i)?'T':'F')
#define fprintfIfc(c) ((c)?\
                       fprintf (stderr, ",\"%c\"",c):\
                       fprintf (stderr, ",\"\""))
void DumpErr(void)
{
    fprintf (stderr,  "\"%s\"",Process.ID);
    fprintf (stderr, ",\"%s\"",Process.info.visteam);
    fprintf (stderr, ",%1d",Process.Inning);
    fprintf (stderr, ",%1d",Process.Bottom);
    fprintf (stderr, ",%1d",Process.Outs);
    fprintf (stderr, ",%1d",Process.Balls);
    fprintf (stderr, ",%1d",Process.Strikes);
    fprintf (stderr, ",\"%s\"",Process.Pitches);
    fprintf (stderr, ",%1d",Process.Score[0]);
    fprintf (stderr, ",%1d",Process.Score[1]);
    fprintf (stderr, ",\"%s\"",Process.Bases[0]);
    fprintf (stderr, ",\"%c\"",evRosterBats(Process.Bases[0]));
    fprintf (stderr, ",\"%s\"",Process.Bases[0]);
    fprintf (stderr, ",\"%c\"",evRosterBats(Process.Bases[0]));
    fprintf (stderr, ",\"%s\"",Process.Defense[1-Process.Bottom][0]);
    fprintf (stderr, ",\"%c\"",evRosterThrows(Process.Defense[1-Process.Bottom][0]));
    fprintf (stderr, ",\"%s\"",Process.Defense[1-Process.Bottom][0]);
    fprintf (stderr, ",\"%c\"",evRosterThrows(Process.Defense[1-Process.Bottom][0]));
    fprintf (stderr, ",\"%s\"",Process.Defense[1-Process.Bottom][1]);
    fprintf (stderr, ",\"%s\"",Process.Defense[1-Process.Bottom][2]);
    fprintf (stderr, ",\"%s\"",Process.Defense[1-Process.Bottom][3]);
    fprintf (stderr, ",\"%s\"",Process.Defense[1-Process.Bottom][4]);
    fprintf (stderr, ",\"%s\"",Process.Defense[1-Process.Bottom][5]);
    fprintf (stderr, ",\"%s\"",Process.Defense[1-Process.Bottom][6]);
    fprintf (stderr, ",\"%s\"",Process.Defense[1-Process.Bottom][7]);
    fprintf (stderr, ",\"%s\"",Process.Defense[1-Process.Bottom][8]);
    fprintf (stderr, ",\"%s\"",NZ(Process.Bases[1], ""));
    fprintf (stderr, ",\"%s\"",NZ(Process.Bases[2], ""));
    fprintf (stderr, ",\"%s\"",NZ(Process.Bases[3], ""));
    fprintf (stderr, ",\"%s\"",Process.EventText);
    fprintf (stderr, ",\"%c\"",TF(Process.isLeadoff));
    fprintf (stderr, ",\"%c\"",TF(Process.isPinchHit));
    fprintf (stderr, ",%d",Process.isPinchHit?11:evFindDefense(Process.Bottom, Process.Bases[0])+1);
    fprintf (stderr, ",%d",evFindLineup(Process.Bottom, Process.Bases[0])+1);
    fprintf (stderr, ",%d",evEventResults[Process.Event].EventType);
    fprintf (stderr, ",\"%c\"",TF(Process.batterEventFlag)); /* TODO BatterEventFlag */
    fprintf (stderr, ",\"%c\"",TF(Process.ABFlag)); /* TODO ABFlag */
    fprintf (stderr, ",%d",Process.Event == evHomeRun?4:
                           Process.Event == evTriple?3:
                           Process.Event == evDouble?2:
                           Process.Event == evSingle?1:0);
    fprintf (stderr, ",\"%c\"",TF(Process.SHFlag)); /* TODO SH Flag */
    fprintf (stderr, ",\"%c\"",TF(Process.SFFlag)); /* TODO SF Flag */
    fprintf (stderr, ",%d"    ,Process.OutsOnPlay ); /* TODO Outs On Play */
    fprintf (stderr, ",\"%c\"",TF(Process.DoublePlayFlag)); /* TODO DoublePlay Flag */
    fprintf (stderr, ",\"%c\"",TF(Process.TriplePlayFlag)); /* TODO TriplePlay Flag */
    fprintf (stderr, ",%d"    ,Process.RBI ); /* TODO RBI On Play */
    fprintf (stderr, ",\"%c\"",TF(Process.WildPitchFlag)); /* TODO WildPitch Flag */
    fprintf (stderr, ",\"%c\"",TF(Process.PassedBallFlag)); /* TODO PassedBall Flag */
    fprintf (stderr, ",%c"    ,Process.FieldedBy);
    fprintfIfc(Process.BattedBall);
    fprintf (stderr, ",\"%c\"",TF(Process.BuntFlag)); /* TODO Bunt Flag */
    fprintf (stderr, ",\"%c\"",TF(Process.FoulFlag)); /* TODO Foul Flag */
    fprintf (stderr, ",\"%s\"",Process.HitLocation); /* TODO HitLocation */
    fprintf (stderr, ",%d"    ,Process.NumErrors); /* TODO NumErrors */
    fprintf (stderr, ",%d"    ,Process.Error[0].Player); /* TODO Error1 Player */
    fprintf (stderr, ",\"%c\"",NZ(Process.Error[0].Type,'N')); /* TODO Error1 Type */
    fprintf (stderr, ",%d"    ,Process.Error[1].Player); /* TODO Error2 Player */
    fprintf (stderr, ",\"%c\"",NZ(Process.Error[1].Type,'N')); /* TODO Error2 Type */
    fprintf (stderr, ",%d"    ,Process.Error[2].Player); /* TODO Error3 Player */
    fprintf (stderr, ",\"%c\"",NZ(Process.Error[2].Type,'N')); /* TODO Error3 Type */
    fprintf (stderr, ",%d"    ,Process.Dest[0]);
    fprintf (stderr, ",%d"    ,Process.Dest[1]);
    fprintf (stderr, ",%d"    ,Process.Dest[2]);
    fprintf (stderr, ",%d"    ,Process.Dest[3]);
    fprintf (stderr, ",\"%s\"",Process.PlayOn[0]);
    fprintf (stderr, ",\"%s\"",Process.PlayOn[1]);
    fprintf (stderr, ",\"%s\"",Process.PlayOn[2]);
    fprintf (stderr, ",\"%s\"",Process.PlayOn[3]);
    fprintf (stderr, ",\"%c\"",TF(Process.SB[0])); /* TODO SB1 Flag */
    fprintf (stderr, ",\"%c\"",TF(Process.SB[1])); /* TODO SB2 Flag */
    fprintf (stderr, ",\"%c\"",TF(Process.SB[2])); /* TODO SB3 Flag */
    fprintf (stderr, ",\"%c\"",TF(Process.CS[0])); /* TODO CS1 Flag */
    fprintf (stderr, ",\"%c\"",TF(Process.CS[1])); /* TODO CS2 Flag */
    fprintf (stderr, ",\"%c\"",TF(Process.CS[2])); /* TODO CS3 Flag */
    fprintf (stderr, ",\"%c\"",TF(Process.PO[0])); /* TODO PO1 Flag */
    fprintf (stderr, ",\"%c\"",TF(Process.PO[1])); /* TODO PO2 Flag */
    fprintf (stderr, ",\"%c\"",TF(Process.PO[2])); /* TODO PO3 Flag */
    fprintf (stderr, ",\"%s\"",NZ(Process.Resp[0],""));
    fprintf (stderr, ",\"%s\"",NZ(Process.Resp[1],""));
    fprintf (stderr, ",\"%s\"",NZ(Process.Resp[2],""));
    fprintf (stderr, ",\"%c\"",TF(Process.NewGameFlag));
    fprintf (stderr, ",\"%c\"",TF(Process.EndGameFlag)); /* TODO EndGame Flag */
    fprintf (stderr, ",\"%c\"",TF(Process.Pinch[0])); /* TODO Pinch1 */
    fprintf (stderr, ",\"%c\"",TF(Process.Pinch[1])); /* TODO Pinch2 */
    fprintf (stderr, ",\"%c\"",TF(Process.Pinch[2])); /* TODO Pinch3 */
    fprintf (stderr, ",\"%s\"",Process.Removed[1]); /* TODO Removed1 */
    fprintf (stderr, ",\"%s\"",Process.Removed[2]); /* TODO Removed2 */
    fprintf (stderr, ",\"%s\"",Process.Removed[3]); /* TODO Removed3 */
    fprintf (stderr, ",\"%s\"",Process.Removed[0]); /* TODO RemovedBatter */
    fprintf (stderr, ",%d"    ,Process.RemovedBatPos); /* TODO RemovedBatter Pos */
    fprintf (stderr, ",%d"    ,Process.Putout[0]);
    fprintf (stderr, ",%d"    ,Process.Putout[1]);
    fprintf (stderr, ",%d"    ,Process.Putout[2]);
    fprintf (stderr, ",%d"    ,Process.Assist[0]);
    fprintf (stderr, ",%d"    ,Process.Assist[1]);
    fprintf (stderr, ",%d"    ,Process.Assist[2]);
    fprintf (stderr, ",%d"    ,Process.Assist[3]);
    fprintf (stderr, ",%d"    ,Process.Assist[4]);
    fprintf (stderr, ",%d"    ,Process.EventNum);
    fprintf (stderr, "\n");
}
void evProcessPlay (struct evPlay *p)
{
    char EventCopy[100];
    Process.Bases[0] = evFindPlayer(p->ID);
    int ival;
    sscanf(p->Inning, "%d", &ival);
    Process.Inning    = ival;
    Process.Bottom    = p->IsHome[0] - '0';
    Process.Balls     = p->Count[0]  - '0';
    Process.Strikes   = p->Count[1]  - '0';
    Process.Pitches   = p->Pitches;
    Process.EventText = p->Event;
    strcpy(EventCopy, p->Event);
    char* slash = strchr(EventCopy, '/');
    if (slash) *slash = 0;
    char* period = strchr(EventCopy, '.');
    if (period) *period = 0;
    evParseEventFull(p->Event);
    Process.Event     = evParseEvent(EventCopy);
    Process.batterEventFlag = evEventResults[Process.Event].BatterEventFlag;
    Process.OutsOnPlay      = evEventResults[Process.Event].OutsOnPlay;
    Process.ABFlag          = evEventResults[Process.Event].AB;
    int Advance[4];
    Advance[0] = evEventResults[Process.Event].ForcedAdvance;
    Advance[1] = Advance[0] - (Process.Bases[1] == 0);
    Advance[2] = Advance[1] - (Process.Bases[2] == 0);
    Advance[3] = Advance[2] - (Process.Bases[3] == 0);
    Process.Dest[0] = Advance[0];
    Process.Dest[1] = Process.Bases[1]?1+Advance[1]:0;
    Process.Dest[2] = Process.Bases[2]?2+Advance[2]:0;
    Process.Dest[3] = Process.Bases[3]?3+Advance[3]:0;
    Process.Resp[0] = Process.Bases[1]?Process.Defense[!Process.Bottom][0]:0;
    Process.Resp[1] = Process.Bases[2]?Process.Defense[!Process.Bottom][0]:0;
    Process.Resp[2] = Process.Bases[3]?Process.Defense[!Process.Bottom][0]:0;
    Process.EventNum++;
    switch (Process.Event)
    {
        case evAssisted: Process.FieldedBy = EventCopy[0];
                         strcpy (Process.PlayOn[0], EventCopy);
                         Process.Putout[0] = EventCopy[1]-'0';
                         Process.Assist[0] = EventCopy[0]-'0'; break;
        case evUnassist: Process.FieldedBy = EventCopy[0];
                         strcpy (Process.PlayOn[0], EventCopy);
                         Process.Putout[0] = EventCopy[0]-'0';
                         Process.Assist[0] = 0;                break;
        default:         Process.FieldedBy = '0';
                         Process.PlayOn[0][0] = 0;
                         Process.Putout[0] = 0;                break;
    }
    if (slash)
    {
        Process.BattedBall = slash[1];
        strcpy(Process.HitLocation, slash+2);
    }
    else
    {
        Process.BattedBall = 0;
        Process.HitLocation[0] = 0;
    }
    DumpErr();
    Process.NewGameFlag = 0;
    if (Advance[3])
    {
        Process.Bases[3] = 0;
        Process.Resp[2] = 0;
    }
    if (Advance[2])
    {
        if (Advance[2] == 1)
        {
            Process.Bases[3] = Process.Bases[2];
            Process.Resp[2] = Process.Resp[1];
        }
        Process.Bases[2] = 0;
        Process.Resp[1] = 0;
    }
    if (Advance[1])
    {
        if (Advance[1] < 3)
        {
            Process.Bases[1+Advance[1]] = Process.Bases[1];
            Process.Resp[Advance[1]] = Process.Resp[0];
        }
        Process.Bases[1] = 0;
        Process.Resp[0] = 0;
    }
    if (Advance[0])
    {
        if (Advance[0] < 4)
        {
            Process.Bases[Advance[0]] = Process.Bases[0];
            Process.Resp[Advance[0]-1] = Process.Defense[!Process.Bottom][0];
        }
    }
    Process.isLeadoff = 0;
    Process.Outs += Process.OutsOnPlay;
    if (Process.Outs >= 3)
    {
        if (Process.Bottom) Process.Inning++;
        Process.Bottom ^= 1;
        Process.isLeadoff = 1;
        Process.Outs = 0;
        Process.Bases[0] = 0;
        Process.Bases[1] = 0;
        Process.Bases[2] = 0;
        Process.Bases[3] = 0;
        Process.Assist[0] = 0;
    }
}
void evProcessLRAdj (struct evLRAdj *p)
{
    /*
    printf ("IsHome     -> %s\n", p->IsHome);
    printf ("Pos        -> %s\n", p->Pos);
    */
}
void evProcessStartSub(struct evStartSub *p)
{
    int iIsHome = *p->IsHome - '0';
    char* found = evFindPlayer(p->ID);
    if (!found)
    {
        found = strcpy (Process.Players[Process.NumPlayers], p->ID);
        Process.NumPlayers++;
    }
    /* BatOrder is 1 digit */
    Process.Lineups[iIsHome][*p->BatOrder-'1'] = found;
    /* FieldPos can be 2 digit */
    int ival;
    sscanf (p->FieldPos, "%d", &ival);
    Process.Defense[iIsHome][ival        - 1 ] = found;
}
void evProcessVersion (struct evVersion *p)
{
    assert(p->Value[0] == '1');
}
void evProcess(struct ev *pev)
{
    switch (pev->type)
    {
    case evBAdj:    evProcessBPAdj   (&pev->data.BPAdj);    break;
    case evCom:     evProcessCom     (&pev->data.Com);      break;
    case evData:    evProcessData    (&pev->data.Data);     break;
    case evID:      evProcessID      (&pev->data.ID);       break;
    case evInfo:    evProcessInfo    (&pev->data.Info);     break;
    case evLAdj:    evProcessLAdj    (&pev->data.LRAdj);    break;
    case evPAdj:    evProcessBPAdj   (&pev->data.BPAdj);    break;
    case evPRAdj:   evProcessPRAdj   (&pev->data.PRAdj);    break;
    case evPlay:    evProcessPlay    (&pev->data.Play);     break;
    case evRAdj:    evProcessLRAdj   (&pev->data.LRAdj);    break;
    case evStart:   evProcessStartSub(&pev->data.StartSub); break;
    case evSub:     evProcessStartSub(&pev->data.StartSub); break;
    case evVersion: evProcessVersion (&pev->data.Version);  break;
    case evUNK:     printf ("unknown\n");                   break;
    default:        printf ("default\n");                   break;
    }
}

#define MAX_ROSTER (50*30)
int NumRoster = 0;
struct evRoster Roster[MAX_ROSTER];
struct evRoster* evFindRoster(char* id)
{
    struct evRoster* found = 0;
    for (int i=0; i<NumRoster && !found; i++)
    {
        if (!strcmp(id, Roster[i].ID))
            found = &Roster[i];
    }
    return found;
}
void evRosterAdd(char* s)
{
    char* c = strchr(s, ',');
    *c = 0;
    struct evRoster* r = evFindRoster(s);
    if (!r && (NumRoster < MAX_ROSTER))
    {
        strcpy(Roster[NumRoster].ID       , s); s = c+1; c = strchr(s, ','); *c = 0;
        strcpy(Roster[NumRoster].LastName , s); s = c+1; c = strchr(s, ','); *c = 0;
        strcpy(Roster[NumRoster].FirstName, s); s = c+1; c = strchr(s, ','); *c = 0;
        Roster[NumRoster].Bat            = *s ; s = c+1; c = strchr(s, ','); *c = 0;
        Roster[NumRoster].Throw          = *s ; s = c+1; c = strchr(s, ','); *c = 0;
        strcpy(Roster[NumRoster].Team     , s); s = c+1;
        strcpy(Roster[NumRoster].Position , s);
        NumRoster++;
    }
    else
    {
        //printf ("dup/full: %s %s\n", s, c+1);
    }
}
char evRosterBats(char* id)
{
    char retval = 0;
    struct evRoster* r = evFindRoster(id);
    if (r) retval = (r->Bat=='R')?'R':'L';
    return retval;
}
char evRosterThrows(char* id)
{
    char retval = 0;
    struct evRoster* r = evFindRoster(id);
    if (r) retval = (r->Throw=='R')?'R':'L';
    return retval;
}
void evRosterDump(void)
{
    for (int i=0; i<NumRoster; i++)
    {
        printf ("%s", Roster[i].ID);
        printf (",%s", Roster[i].LastName);
        printf (",%s", Roster[i].FirstName);
        printf (",%c", Roster[i].Bat);
        printf (",%c", Roster[i].Throw);
        printf (",%s", Roster[i].Team);
        printf (",%s\n", Roster[i].Position);
    }
}