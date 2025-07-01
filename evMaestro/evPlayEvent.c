#include "evPlayEvent.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
/* https://www.retrosheet.org/eventfile.htm */
/* https://www.retrosheet.org/datause.html */

#if 0
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

#define NUM_ASSISTS (5)
struct EventDataType
{
    enum EventType mEv;
    struct
    {
        unsigned char mNum;
        unsigned char mList[NUM_ASSISTS];
    } mAssist;
}

struct EventDataType
{
    enum EventType          mEv;
    struct ModifierListType mList;
};
#endif
const static struct evEventConfigType
{
    char*         String;
    unsigned char Defense;
    unsigned char BatterEventFlag;
    unsigned char OutsOnPlay;
    unsigned char Advance;
    unsigned char EventType;
    unsigned char AB;
} evEventConfig[] =
{
                         /*   {String                     Defense, BatterEventFlag OutsOnPlay Advance EventType AB}*/
//[evUnknownEvent]          = {"evUnknownEvent",             0,          1        ,     0    ,   0   ,     0   , 0},
  [evNoEvent]               = {"evNoEvent",                  0,          0        ,     0    ,   0   ,     1   , 0},
//[evGenericOut]            = {"evGenericOut",               0,          1        ,     1    ,   0   ,     2   , 1},
  [evStrikeout]             = {"evStrikeout",                0,          1        ,     1    ,   0   ,     3   , 1},
  [evStolenBase]            = {"evStolenBase",               0,          0        ,     0    ,   0   ,     4   , 0},
  [evDefensiveIndifference] = {"evDefensiveIndifference",    0,          0        ,     0    ,   0   ,     5   , 0},
  [evCaughtStealing]        = {"evCaughtStealing",           0,          0        ,     1    ,   0   ,     6   , 0},
//[evPickoffError]          = {"evPickoffError",             0,          0        ,     0    ,   0   ,     7   , 0},
  [evPickoff]               = {"evPickoff",                  0,          0        ,     1    ,   0   ,     8   , 0},
  [evWildPitch]             = {"evWildPitch",                0,          0        ,     0    ,   0   ,     9   , 0},
  [evPassedBall]            = {"evPassedBall",               0,          0        ,     0    ,   0   ,    10   , 0},
  [evBalk]                  = {"evBalk",                     0,          0        ,     0    ,   0   ,    11   , 0},
  [evOtherAdvance]          = {"evOtherAdvance",             0,          0        ,     0    ,   1   ,    12   , 0},
  [evFoulError]             = {"evFoulError",                0,          0        ,     0    ,   0   ,    13   , 0},
  [evWalk]                  = {"evWalk",                     0,          1        ,     0    ,   1   ,    14   , 0},
  [evIntentionalWalk]       = {"evIntentionalWalk",          0,          1        ,     0    ,   1   ,    15   , 0},
  [evHitByPitch]            = {"evHitByPitch",               0,          1        ,     0    ,   1   ,    16   , 0},
  [evInterference]          = {"evInterference",             0,          1        ,     0    ,   1   ,    17   , 0},
//[evError]                 = {"evError",                    0,          1        ,     0    ,   1   ,    18   , 1},
  [evFieldersChoice]        = {"evFieldersChoice",           0,          1        ,     1    ,   0   ,    19   , 0},
  [evSingle]                = {"evSingle",                   1,          1        ,     0    ,   1   ,    20   , 1},
  [evDouble]                = {"evDouble",                   1,          1        ,     0    ,   2   ,    21   , 1},
  [evTriple]                = {"evTriple",                   1,          1        ,     0    ,   3   ,    22   , 1},
  [evHomeRun]               = {"evHomeRun",                  0,          1        ,     0    ,   4   ,    23   , 1},
//"evMissingPlay",
//"evUnassist",
//"evAssisted",
//[evLinedDoublePlay]       = {"evLinedDoublePlay",          0,          1        ,     2    ,       0     ,    27,    1},
  [evGroundRuleDouble]      = {"evGroundRuleDouble",         0,          1        ,     0    ,       2     ,    21,    1},
  [evPickoffCaughtStealing] = {"evPickoffCaughtStealing",    0,          0        ,     1    ,       0     ,     8,    0},
  [evNumEvents]             = {"evNumEvents",                0,          1        ,     1    ,       0     ,     8,    0},
};
char* evString (enum evPlayEventType ev)
{
    return evEventConfig[ev].String;
}
const struct EventFilterType
{
    char*                mMatch;
    enum evPlayEventType mEv;
} EventFilters[] =
{
    {"POCS", evPickoffCaughtStealing},

    {"DGR" , evGroundRuleDouble},

    {"FL"  , evFoulError},
    {"BK"  , evBalk},
    {"CS"  , evCaughtStealing},
    {"DI"  , evDefensiveIndifference},
    {"FC"  , evFieldersChoice},
    {"HP"  , evHitByPitch},
    {"HR"  , evHomeRun},
    {"IW"  , evIntentionalWalk},
    {"NP"  , evNoEvent},
    {"OA"  , evOtherAdvance},
    {"PB"  , evPassedBall},
    {"PO"  , evPickoff},
    {"SB"  , evStolenBase},
    {"WP"  , evWildPitch},

    //{"E"   , evError},
    {"C"   , evInterference},
    {"D"   , evDouble},
    {"H"   , evHomeRun},
    {"I"   , evIntentionalWalk},
    {"K"   , evStrikeout},
    {"S"   , evSingle},
    {"T"   , evTriple},
    {"W"   , evWalk},
};
#define NUM_FILTERS (sizeof(EventFilters)/sizeof(EventFilters[0]))

char* evParseEvent (char* str, enum evPlayEventType* ev)
{
    *ev = evNumEvents;
    for (int i=0; (i<NUM_FILTERS) && (*ev==evNumEvents); i++)
    {
        if (strstr(str,EventFilters[i].mMatch) == str)
        {
            *ev = EventFilters[i].mEv;
            str += strlen(EventFilters[i].mMatch);
            str += evEventConfig[*ev].Defense;
        }
    }
    return str;
}

#define Cvt(c,e) ((((c)=='B')?0:\
                   ((c)=='H')?4:\
                   ((c)=='X')?255:\
                   ((c)-'0')) | \
                   ((e)?evErrorInvolved:0x00))
void evProcessAdvance (struct evParseType* p, char* str)
{
    /* advance := N-M */
    char from = Cvt(str[0], 0);
    assert (str[1] == '-');
    char to = Cvt(str[2], 0);
    p->mResult[p->mNumRes].mFrom = from;
    p->mResult[p->mNumRes].mTo   = to;
    p->mNumRes++;
}
char* evProcessEvent (struct evParseType* p, char* str)
{
    str = evParseEvent (str, &p->mEv[p->mNumEv]);
    p->mResult[0].mFrom = 0;
    p->mResult[0].mTo = evEventConfig[p->mEv[p->mNumEv]].Advance;
    p->mNumRes += (evEventConfig[p->mEv[p->mNumEv]].Advance != 0);
    char error = 0;
    char done = 0;
    switch (p->mEv[p->mNumEv])
    {
        case evNumEvents:
            while (!done)
            {
                switch (*str)
                {
                    case 'E': error = 1; break;
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9': p->mResult[0].mVia[p->mResult[0].mNumVia] = Cvt(*str,error);
                                p->mResult[0].mNumVia++;
                                error = 0;
                                break;
                    case '(': p->mResult[0].mFrom = Cvt(str[1], 0);
                                p->mResult[0].mTo   = 0xFF;
                                str += 2;
                                break;
                }
                str++;
            }
            for (;(*str > '0') && (*str <= '9'); str++)
            {
                if (*str == 'E') error = 1;
                else p->mResult[0].mVia[p->mResult[0].mNumVia++] = (*str) - '0';
            }
            if (*str=='(')
            {
                p->mResult[0].mFrom = Cvt(str[1], error);
                p->mResult[0].mTo = 0xFF;
            }
        break;
    }
    p->mNumEv++;
    return str;
}

char* evParse (char* str, struct evParseType *p)
{
    /* event+event/modifier/modifier.advance;advance\x00 */
    memset (p, 0, sizeof(struct evParseType));
    //char* mod0 = strchr(str,'/');

    /* ****************************** */
    /* Event(s) */
    do
    {
        str = evProcessEvent(p, str);
    } while (*str == '+');
    /* ****************************** */

    /* ****************************** */
    /* Advances */
    str = (*str=='.')?str:0;
    while (str)
    {
        evProcessAdvance(p, str+1);
        str = strchr(str+1,';');
    }
    /* ****************************** */

    return str;
    //assert ((str[0] == '/') || (str[0] == 0));
    char paren = 0;
    char error = 0;
    char done  = (*str == 0);
    switch (p->mEv[0])
    {
        case evStrikeout:
        //case evNumEvents:
            p->mResult[0].mFrom = 0;
            p->mResult[0].mTo = 0xFF;
            break;
        case evWalk:
        case evIntentionalWalk:
            p->mResult[0].mFrom = 0;
            p->mResult[0].mTo = 1;
            break;
        case evHomeRun:
            p->mResult[0].mFrom = 0;
            p->mResult[0].mTo = 4;
            break;
        case evHitByPitch:
            p->mResult[0].mFrom = 0;
            p->mResult[0].mTo = 1;
            break;
        default:
            break;
    }
    while (!done)
    {
        switch (*str)
        {
            case '+': done  = 1; break;
            case '(': paren = 1; break;
            case ')': paren = 0; break;
            case 'E': error = 1; break;
            case 'B': assert(paren);
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case 'H':
                /*
                if (str[1] == '-')
                {
                    char f = Cvt(*str);
                    char t = Cvt(str[2]);
                    p->mBaseRes[f] = t;
                    str += 2;
                }*/
                if (!error &&
                    (paren == evEventConfig[p->mEv[0]].BatterEventFlag))
                {
                    //assert (error == 0x00);
                    switch (p->mEv[0])
                    {
                        case evStolenBase:
                            p->mResult[0].mFrom = Cvt(*str,0)-1;
                            p->mResult[0].mTo = Cvt(*str,0);
                            break;
                        default:
                            p->mResult[0].mFrom = Cvt(*str,0);
                            p->mResult[0].mTo = 0xFF;
                            break;
                    }
                }
                else
                {
                    p->mResult[0].mVia[0] = Cvt(*str,error);
                    error = 0;
                }
                //evProcessEvent(p->mEv, *str);
                break;
            default : assert(0);
        }
        str++;
        done |= (*str == 0);
    }
    //if ((memchr(p->mBaseRes, 0xFF, 4) == 0) && (p->mEv == evNumEvents))
        //p->mBaseRes[0] = 0xFF;
    assert (!paren);
    assert (!error);
    return str;
}

void evDump(struct evParseType *p)
{
    for (int i=0; i<p->mNumEv; i++)
    {
        printf ("mEv: %s\n", evString (p->mEv[i]));
    }
    for (int i=0; i<p->mNumRes; i++)
    {
        printf (" %d -> %d", p->mResult[i].mFrom, p->mResult[i].mTo);
        for (int j=0; j<p->mResult->mNumVia; j++)
        {
            printf (" %d", p->mResult[i].mVia[j]);
        }
        printf ("\n");
    }
}