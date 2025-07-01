#include "evGame.h"
#include "evGameID.h"
#include "evVersion.h"
#include "evInfo.h"
#include "evStartSub.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>

static TeamType home;
struct GameStartType
{
    struct evRosterType*  roster;
    struct evStartSubType start;
};
//static struct GameStartType Starts[2][9];
void evGameInit(struct evGame* pg)
{
    memset (pg, 0, sizeof (struct evGame));
    pg->inning = 1;
    pg->top = TRUE;
    pg->outs = 0;
    pg->balls = 0;
    pg->strikes = 0;
    pg->visscore = 0;
    pg->homescore = 0;
    pg->leadoffflag = TRUE;
    pg->lineupposition = 1;
    pg->newgameflag = TRUE;
    pg->eventnum = 0;
};

void evSetHome(struct evGame* pg, TeamType h)
{
    strcpy(home, h);
}

enum evGameEvents
{
    /* $ = [1-9] */
    /* % = [123H] */
    Unassist,              /* [1-9]$                    A single fielder represents an unassisted out made by the specified fielder. Modifiers can be added to indicate the batted ball trajectory: G for ground ball, L for line drive, P for pop up, F for a fly ball BG for bunt grounder, BP for bunt pop up. The ball trajectory code may be followed by a hit location code. */
    Assisted,              /* [1-9]{2}$                 Strings of two or more fielders as an event specify a ground out where the put out is credited by the last fielder in the string. Other fielders are credited with assists. */
    GroundDoublePlay,      /* [1-9][1-9]?([123H])[1-9]$ Events oftisorm are used to code grounded into double plays. */
    LinedDoublePlay,       /* [1-9](B)[1-9]([123H])     followed by the modifier LDP is used to indicate a lined into double play. */
    Interference,          /* C$                        codes catcher interference. Implicitly, the batter is awarded first unless overridden by an advance indicating otherwise. A redundant B-1 is allowed. */
    Single,                /* S[1-9]$                   single */
    Double,                /* D[1-9]$                   double */
    Triple,                /* T[1-9]$                   triple */
    GroundruleDouble,      /* DGR                       is the code for a ground rule double. No fielding player is specified. */
    ReachOnError,          /* E[1-9]$                   is the code for an error allowing a batter to get on base. The fielder making the error is given by $. The batter advance to first is implicit but may be given explicitly. */
    FieldersChoice,        /* FC[1-9]$                  Fielder's choice. $ is the fielder first fielding the ball. The batter advance to first is understood if it is not given explicitly. */
    FoulError,             /* FLE[1-9]$                 Error on foul fly ball. */
    HomeRun,               /* HR?$                      is the code for a home run leaving the park. The location modifier can be used to indicate where the ball left the playing field. */
    InsideHomeRun,         /* HR?[1-9]$                 indicates an inside-the-park home run by giving a fielder as part of the code. */
    HitByPitch,            /* HP$                       Batter hit by a pitch. The batter advance to first is implicit. Other advances are given if needed. */
    StrikeOut,             /* K$                        Strike out */
    NoPlay,                /* NP$                       no play. This event is used as a marker when substitutions are made. */
    IntentionalWalk,       /* IW?$                      intentional walk */
    Walk,                  /* W$                        walk. In both cases base runner advances are given if needed. The batter advance to first base is implicit. */

    Balk,                  /* BK$                       indicates a balk. */
    CaughtStealing,        /* CS[123H]([1-9][1-9])$     is the event code for caught stealing. The bases, %, for this play are 2,3 and H. The fielding data, $$, is considered part of the play. Other advances may be given. */
    DefensiveIndifference, /* DI$                       is the defensive indifference code and is given when there is no attempt to prevent a stolen base. The advance field specifies which base the runner went to. */
    RunnerAdvance,         /* OA$                       is coded for a base runner advance that is not covered by one of the other codes. A comment may be given explaining the advance. */
    PassedBall,            /* PB$                       passed ball */
    WildPitch,             /* WP$                       wild pitch. In both cases the catcher is unable to handle a pitch and a base runner advances. */
    PickOff,               /* PO[123H]([1-9][1-9])$     picked off of base % (1, 2 or 3) with the ($$) indicating the throw(s) and fielder making the putout. */
    PickOffCaughtStealing, /* POCS[123H]([1-9][1-9])$   picked off off base % (2, 3 or H) with the runner charged with a caught stealing. The ($$) is the sequence of throws resulting in the out. */
    StolenBase,            /* SB[123H]$                 is the event code for a stolen base. The bases, %, for this play are 2,3 and H. */

    Uninit = -1
};
const char* labels[] =
{
    "Unassist",
    "Assisted",
    "GroundDoublePlay",
    "LinedDoublePlay",
    "Interference",
    "Single",
    "Double",
    "Triple",
    "GroundruleDouble",
    "ReachOnError",
    "FieldersChoice",
    "FoulError",
    "HomeRun",
    "InsideHomeRun",
    "HitByPitch",
    "StrikeOut",
    "NoPlay",
    "IntentionalWalk",
    "Walk",
    "Balk",
    "CaughtStealing",
    "DefensiveIndifference",
    "RunnerAdvance",
    "PassedBall",
    "WildPitch",
    "PickOff",
    "PickOffCaughtStealing",
    "StolenBase"
};
#define MIN(a,b) (((a)<(b))?(a):(b))
static void ParseDescription (char* str, enum evGameEvents *ev)
{
    char* additional_plus = strchr (str, '+');
    char* additional_semi = strchr (str, ';');
    char* additional;
    if      (!additional_plus) additional=additional_semi;
    else if (!additional_semi) additional=additional_plus;
    else                       additional = MIN(additional_plus, additional_semi);
    if (additional)
    {
        *additional = 0;
        additional++;
    }
         if (!strncmp (str, "POCS", 4)) *ev = PickOffCaughtStealing;
    else if (!strncmp (str, "FLE" , 3)) *ev = FoulError;
    else if (!strncmp (str, "DGR" , 3)) *ev = GroundruleDouble;
    else if (!strncmp (str, "PO"  , 2)) *ev = PickOff;
    else if (!strncmp (str, "CS"  , 2)) *ev = CaughtStealing;
    else if (!strncmp (str, "BK"  , 2)) *ev = Balk;
    else if (!strncmp (str, "DI"  , 2)) *ev = DefensiveIndifference;
    else if (!strncmp (str, "OA"  , 2)) *ev = RunnerAdvance;
    else if (!strncmp (str, "PB"  , 2)) *ev = PassedBall;
    else if (!strncmp (str, "WP"  , 2)) *ev = WildPitch;
    else if (!strncmp (str, "SB"  , 2)) *ev = StolenBase;
    else if (!strncmp (str, "HP"  , 2)) *ev = HitByPitch;
    else if (!strncmp (str, "NP"  , 2)) *ev = NoPlay;
    else if (!strncmp (str, "FC"  , 2)) *ev = FieldersChoice;
    else if (*str == 'C')               *ev = Interference;
    else if (*str == 'S')               *ev = Single;
    else if (*str == 'D')               *ev = Double;
    else if (*str == 'T')               *ev = Triple;
    else if (*str == 'E')               *ev = ReachOnError;
    else if (*str == 'H')               *ev = HomeRun;
    else if (*str == 'I')               *ev = IntentionalWalk;
    else if (*str == 'W')               *ev = Walk;
    else if (*str == 'K')               *ev = StrikeOut;
    else if (strlen(str) == 1)          *ev = Unassist;
    else if (strlen(str) == 2)          *ev = Assisted;
    else if (strchr(str, 'B'))          *ev = LinedDoublePlay;
    else                                *ev = GroundDoublePlay;
    if (additional)
    {
        ParseDescription(additional, ev+1);
    }
}
#define NUM_EVENTS (4)
void evSetPlay(struct evGame* pg, struct evPlayType *p)
{
    enum evGameEvents events[NUM_EVENTS];
    memset (events, 0xFF, sizeof (enum evGameEvents) * NUM_EVENTS);
    //assert (pg->inning == p->Inning);
    //assert (pg->top != p->Home);
    pg->batter = evRoster_Get(p->player);
    pg->balls = p->Count/10;
    pg->strikes = p->Count%10;
    strcpy (pg->pitchsequence, p->Pitches);
    strcpy (pg->eventtext, p->Event);
    pg->outs += pg->outsonplay;

    char* description = pg->eventtext;
    char* modifiers = strchr(description, '/');
    char* advances = strchr(description, '.');
    if (modifiers) { *modifiers = 0; modifiers++; }
    if (advances)  { *advances  = 0; advances++; }
    ParseDescription (description, events);
    assert (events[NUM_EVENTS-1] == Uninit);
    printf ("Events: ");
    for (int i=0; (events[i] != -1) && i<NUM_EVENTS; i++)
    {
        printf ("%s, ", labels[events[i]]);
    }
    printf ("\n");
}

/* https://www.retrosheet.org/eventfile.htm
Events made by the batter at the plate
$ is defense
% is base

$ A single fielder represents an unassisted out made by the specified fielder. Modifiers can be added to indicate the batted ball trajectory: G for ground ball, L for line drive, P for pop up, F for a fly ball BG for bunt grounder, BP for bunt pop up. The ball trajectory code may be followed by a hit location code.
    play,7,0,saboc001,01,CX,8/F78
    indicates a fly ball caught by the center fielder in left center field.
    A sacrifice fly is indicated by the modifier SF following a fly out play. The runner scoring because of the sacrifice is coded in the advance part of the play.

    play,5,0,grifk001,10,.BX,9/SF.3-H
    In the case that a fielder makes an unassisted out on a ground ball a modifier G follows the event.

    play,5,0,duncm001,00,X,3/G.2-3
    indicates an unassisted out made by the first baseman with the runner on second advancing to third.

$$ Strings of two or more fielders as an event specify a ground out where the put out is credited by the last fielder in the string. Other fielders are credited with assists.
    play,6,0,davie001,01,FX,63/G6M
    indicates a ground ball out at first on a ball fielded by the shortstop.
    play,9,1,pendt001,00,X,143/G1
    More than one player can touch the ball before an out is made. In this case, the pitcher has deflected the ball before the second baseman threw to first base.

    play,7,1,tempg001,00,X,54(B)/BG25/SH.1-2
    If the putout is made at a base not normally covered by the fielder the base runner, batter in this example, is given explicitly.

    Force outs are indicated by adding the FO modifier and indicating the base runner forced.

    play,5,0,gileb001,10,BX,54(1)/FO/G5.3-H;B-1
    The runner on first is forced at second by a throw from the third baseman to the second baseman. The runner on third scores and the batter is safe at first. The explicit advance indicated for the batter is optional. A second modifier is used to indicate the batted ball trajectory and location.
    With the addition of a SH modifier this form is used to indicate sacrifice hits or bunts that advance a runner.

    play,6,1,camik001,00,X,23/SH.1-2

$(%)$ $$(%)$ Events of this form are used to code grounded into double plays.
    play,7,0,backw001,11,FBX,64(1)3/GDP/G6
    indicates a grounded into double play. The parenthesized 1 indicates the base runner on first was the initial out on the play. The GDP modifier is followed by a another / and a hit type and location.
    play,8,1,smito001,22,BFCBX,4(1)3/G4/GDP
    An unassisted ground ball out by the second baseman starts this double play.

$(B)$(%) followed by the modifier LDP is used to indicate a lined into double play.
    play,7,0,leonj001,01,CX,8(B)84(2)/LDP/L8
    indicates a line drive out to the center fielder with the runner on second doubled up.
    play,7,0,fernt001,10,BX,3(B)3(1)/LDP
    indicates an unassisted double play by the first baseman who fielded the line drive and caught the runner off first base.

    The double play notation can be extended in obvious ways to describe triple plays.

    play,7,1,randw001,00,.>X,1(B)16(2)63(1)/LTP/L1
    Note: the double digit combination 99, which cannot arise in play, is used to code unknown plays including forms that otherwise describe force outs and the double plays. Additional fielders in the double play are assigned 9. No assist or putout credits are given.

C/E2 codes catcher interference. Implicitly, the batter is awarded first unless overridden by an advance indicating otherwise. A redundant B-1 is allowed.
    play,9,1,cruzj002,??,,C/E2.1-2

C/E1 or C/E3 are used when the pitcher or first baseman are called for interfering with the batter putting him on first without being charged with an at bat. In these cases C is interpreted as interference by the fielder specified following the E, not the catcher.

S$ single
D$ double
T$ triple
    A hit (excepting a home run) is indicated by one of S, D and T optionally followed by the fielder, $, initially handling the ball. If more than one fielder handles the ball the appropriate sequence of fielders is given. The fielder designation is omitted if that information is not known. The batter advance to the designated base is implicit.

    play,8,0,pacit001,??,,S7
    is a minimal coding of a single showing that the left fielder first handled the ball. The ?? in the count field indicates the count at the time of the hit is unknown.
    play,2,1,santn001,12,CFBX,D7/G5.3-H;2-H;1-H
    codes a bases loaded double fielded by the left fielder, a modifier showing the hit location code and advances for each of the base runners.

    play,3,0,raint001,11,CBX,T9/F9LD.2-H
    describes a triple to right field, a hit location and a runner on second scoring.

DGR is the code for a ground rule double. No fielding player is specified.
    play,3,0,surhb001,10,.BX,DGR/L9LS.2-H

E$ is the code for an error allowing a batter to get on base. The fielder making the error is given by $. The batter advance to first is implicit but may be given explicitly.
    play,2,0,ruffb001,10,BX,E1/TH/BG15.1-3
    indicates a throwing error (modifier "/TH") error on the pitcher with the runner on first advancing to third. The batter advance to first is implicit.
    play,5,1,young001,00,X,E3.1-2;B-1
    indicates a fielding error by the first baseman. In this case the batter advance to first has been explicitly given.

FC$ Fielder's choice. $ is the fielder first fielding the ball. The batter advance to first is understood if it is not given explicitly.
    play,4,0,harpb001,22,BBFSFX,FC5/G5.3XH(52)
    The third baseman fielded the ball and threw home in time to retire the runner attempting to score. The batter was safe at first.
    play,5,1,jordr001,00,X,FC3/G3S.3-H;1-2
    The first baseman fielded the ball and attempted to throw an unspecified runner out. No outs were made and the batter is safe at first.

    Note that even though force outs are considered fielder's choices, the notation distinguishes between force outs and non-forced fielder's choices.

FLE$ Error on foul fly ball.
    play,5,0,murre001,00,F,FLE5/P5F

H or HR is the code for a home run leaving the park. The location modifier can be used to indicate where the ball left the playing field.
    play,8,0,bellg001,21,CBBX,H/L7D
    indicates a solo home run to left field.
    play,12,1,bichd001,02,FFFX,HR/F78XD.2-H;1-H
    shows a home run to center field with the runners on first and second scoring.

H$ or HR$ indicates an inside-the-park home run by giving a fielder as part of the code.
    play,4,0,younr001,32,FBFFFBBX,HR9/F9LS.3-H;1-H

HP Batter hit by a pitch. The batter advance to first is implicit. Other advances are given if needed.
    play,1,1,lansc001,00,H,HP.1-2

K Strike out
    play,1,1,steit001,12,C2FBS,K
    play,6,1,wynnm001,22,..BBFCFS,K23
    A dropped third strike with a putout at first base is given by the event K23.

K+event On third strikes various base running play may also occur. The event can be SB%, CS%, OA, PO%, PB, WP and E$.
    play,2,0,roomr001,12,1BF1S11S,K+PB.1-2
    A passed ball on strike three allowed the runner on first to go to second.
    play,5,1,whitd001,02,FLFFS,K+WP.B-1
    An explicit batter advance is given when he reaches first on a third strike miscue. An alternative notation for WP and PB is given below.

    play,8,1,davic001,12,CFB.S,K23+WP.2-3
    Of course, a base running event can occur when the third strike is dropped.

NP no play. This event is used as a marker when substitutions are made.
    play,8,0,puckk001,00,,NP
    sub,kutcr001,"Randy Kutcher",1,5,8

I or IW intentional walk

W walk. In both cases base runner advances are given if needed. The batter advance to first base is implicit.
    play,6,1,ripkc001,32,CFBBFB>B,W.1-2
    play,8,0,sciom001,30,B+22.III,IW

W+event, IW+event. On ball four various base running plays may also occur. The event can be SB%, CS%, PO%, PB, WP and E$.
    play,1,1,sandr001,32,C1FBB.BFB,W+WP.2-3
    The fourth ball was a wild pitch allowing the runner on second to advance.

Base-running events not involving the batter
    The player specified in these plays is the batter at the plate, not the base runner or runners affected by the play.
    The play pitches and count fields (if given) are for the batter at the time of the event. Unless the event is a inning or game ending out it will be followed by another event listing the batter.

BK indicates a balk.
    play,6,0,niekp001,??,,BK.3-H;1-2

CS%($$) is the event code for caught stealing. The bases, %, for this play are 2,3 and H. The fielding data, $$, is considered part of the play. Other advances may be given.
    play,5,1,ceror001,??,,CSH(12)
    play,1,0,bayld001,??,,CS2(24).2-3
    play,6,0,beneb001,??,,CS2(2E4).1-3
    The error negates the out with the advance field indicating a two base advance on the play.

DI is the defensive indifference code and is given when there is no attempt to prevent a stolen base. The advance field specifies which base the runner went to.
    play,9,0,bencj101,??,,DI.1-2

OA is coded for a base runner advance that is not covered by one of the other codes. A comment may be given explaining the advance.
    play,3,1,parkr001,??,,OA.2X3(25)
    com,"Thompson out trying to advance after ball eluded catcher"

PB passed ball

WP wild pitch. In both cases the catcher is unable to handle a pitch and a base runner advances.
    play,1,1,jackb001,12,FBSFFB,WP.2-3;1-2
    play,1,1,evand002,01,CB,PB.2-3

PO%($$) picked off of base % (1, 2 or 3) with the ($$) indicating the throw(s) and fielder making the putout.
    play,4,0,guerp001,00,22,PO2(14)
    indicates the runner on second was out by a pick off throw from the pitcher to second baseman.
    play,1,1,wallt001,10,B11,PO1(E3).1-2
    shows an attempt at a pick off at first with the first baseman committing an error that allows the runner to advance to second. The presence of the error (E3) negates the out normally associated with the pickoff play.

POCS%($$) picked off off base % (2, 3 or H) with the runner charged with a caught stealing. The ($$) is the sequence of throws resulting in the out.
    play,6,1,javis001,10,B1,POCS2(1361)

SB% is the event code for a stolen base. The bases, %, for this play are 2,3 and H.
    play,6,0,benzt001,11,BSB,SB2
    play,4,1,waltj001,10,BB,SB3;SB2
    play,4,1,shefg001,12,SP1CB,SBH;SB2
    show double steals, second and third in one case, second and home in the other.
*/

void evSetVersion (struct evGame* pg, struct evVersionType* aVer);
void evSetInfo (struct evGame* pg, struct evInfoType* aInf);
void evSetID (struct evGame* pg, struct evGameIDType* aID);
boolean evGameProcessLine(struct evGame* pg, char* str)
{
    struct evPlayType    aPlay;
    struct evGameIDType  aID; 
    struct evVersionType aVersion; 
    struct evInfoType    aInfo;
    if (!memcmp(str, ID_STR     , 2)) {evProcessGameID  (str+3, &aID     ); evSetID      (pg, &aID     );}
    if (!memcmp(str, VERSION_STR, 7)) {evProcessVersion (str+8, &aVersion); evSetVersion (pg, &aVersion);}
    if (!memcmp(str, INFO_STR   , 4)) {evProcessInfo    (str+5, &aInfo   ); evSetInfo    (pg, &aInfo   );}
    if (!memcmp(str, PLAY_STR   , 4)) {evProcessPlay    (str+5, &aPlay   ); evSetPlay    (pg, &aPlay   );}
    //if (!memcmp(str, START_STR  , 5)) {struct evStartSubType aStart;   evProcessStartSub(Line+6, &aStart);   DumpStartSub(&aStart);  }
    //if (!memcmp(str, SUB_STR    , 3)) {struct evStartSubType aSub;     evProcessStartSub(Line+4, &aSub);     DumpStartSub(&aSub);    }
    //if (!memcmp(str, DATA_STR   , 4)) {struct evDataType     aData;    evProcessData    (Line+5, &aData);    DumpData    (&aData);   }
    //if (!memcmp(str, COM_STR    , 3));
    //if (!memcmp(str, BADJ_STR   , 4)) {struct evBadjType     aBadj;    evProcessBadj    (Line+5, &aBadj);    DumpBadj    (&aBadj);   }
    return FALSE;
}

void evSetID (struct evGame* pg, struct evGameIDType* aID)
{
    evGameInit(pg);
    //strcpy(pg->game, aID->val);
}

void evSetVersion (struct evGame* pg, struct evVersionType* aVer)
{
}

void evSetInfo (struct evGame* pg, struct evInfoType* aInf)
{
}