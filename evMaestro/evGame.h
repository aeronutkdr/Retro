#ifndef _EV_GAME_H_
#define _EV_GAME_H_
#include "EvPrimitives.h"
#include "EvPlay.h"
/*
#include "EvRoster.h"
#include "EvStartSub.h"
*/

typedef enum
{
    FALSE,
    TRUE
} boolean;
struct evGame
{
    /* number    field */
    /* ------    ----- */
    GameType game;                       /*  0        game id* */
    TeamType visitor;                    /*  1        visiting team* */
    int inning;                          /*  2        inning* */
    boolean top;                         /*  3        batting team* */
    int outs;                            /*  4        outs* */
    int balls;                           /*  5        balls* */
    int strikes;                         /*  6        strikes* */
    char pitchsequence[FIELD_SIZE];      /*  7        pitch sequence */
    int visscore;                        /*  8        vis score* */
    int homescore;                       /*  9        home score* */
    struct evRosterType* batter;         /* 10        batter */
    boolean BatR;                        /* 11        batter hand */
    struct evRosterType* resbatter;      /* 12        res batter* */
    boolean resBatR;                     /* 13        res batter hand* */
    struct evRosterType* pitcher;        /* 14        pitcher */
    boolean pitchR;                      /* 15        pitcher hand */
    struct evRosterType* respitcher;     /* 16        res pitcher* */
    boolean respitchR;                   /* 17        res pitcher hand* */
    struct evRosterType* catcher;        /* 18        catcher */
    struct evRosterType* firstbase;      /* 19        first base */
    struct evRosterType* secondbase;     /* 20        second base */
    struct evRosterType* thirdbase;      /* 21        third base */
    struct evRosterType* shortstop;      /* 22        shortstop */
    struct evRosterType* leftfield;      /* 23        left field */
    struct evRosterType* centerfield;    /* 24        center field */
    struct evRosterType* rightfield;     /* 25        right field */
    struct evRosterType* firstrunner;    /* 26        first runner* */
    struct evRosterType* secondrunner;   /* 27        second runner* */
    struct evRosterType* thirdrunner;    /* 28        third runner* */
    char eventtext[FIELD_SIZE];          /* 29        event text* */
    boolean leadoffflag;                 /* 30        leadoff flag* */
    boolean pinchhitflag;                /* 31        pinchhit flag* */
    int defensiveposition;               /* 32        defensive position* */
    int lineupposition;                  /* 33        lineup position* */
    int eventtype;                       /* 34        event type* */
    boolean battereventflag;             /* 35        batter event flag* */
    boolean abflag;                      /* 36        ab flag* */
    int hitvalue;                        /* 37        hit value* */
    boolean SHflag;                      /* 38        SH flag* */
    boolean SFflag;                      /* 39        SF flag* */
    int outsonplay;                      /* 40        outs on play* */
    boolean doubleplayflag;              /* 41        double play flag */
    boolean tripleplayflag;              /* 42        triple play flag */
    int RBIonplay;                       /* 43        RBI on play* */
    boolean wildpitchflag;               /* 44        wild pitch flag* */
    boolean passedballflag;              /* 45        passed ball flag* */
    struct evRosterType* fieldedby;      /* 46        fielded by */
    char battedballtype;                 /* 47        batted ball type */
    boolean buntflag;                    /* 48        bunt flag */
    boolean foulflag;                    /* 49        foul flag */
    char hitllocation[6];                /* 50        hit location */
    int numerrors;                       /* 51        num errors* */
    struct evRosterType* player1sterror; /* 52        1st error player */
    char type1sterror;                   /* 53        1st error type */
    struct evRosterType* player2nderror; /* 54        2nd error player */
    char type2nderror;                   /* 55        2nd error type */
    struct evRosterType* player3rderror; /* 56        3rd error player */
    char type3rderror;                   /* 57        3rd error type */
    int batterdest;                      /* 58        batter dest* (5 if scores and unearned, 6 if team unearned) */
    int runner1dest;                     /* 59        runner on 1st dest* (5 if scores and unearned, 6 if team unearned) */
    int runner2dest;                     /* 60        runner on 2nd dest* (5 if scores and unearned, 6 if team unearned) */
    int runner3dest;                     /* 61        runner on 3rd dest* (5 if socres and uneanred, 6 if team unearned) */
    char playonbatter[5];                /* 62        play on batter */
    char playonrunner1[5];               /* 63        play on runner on 1st */
    char playonrunner2[5];               /* 64        play on runner on 2nd */
    char playonrunner3[5];               /* 65        play on runner on 3rd */
    boolean SBrunner1;                   /* 66        SB for runner on 1st flag */
    boolean SBrunner2;                   /* 67        SB for runner on 2nd flag */
    boolean SBrunner3;                   /* 68        SB for runner on 3rd flag */
    boolean CSrunner1;                   /* 69        CS for runner on 1st flag */
    boolean CSrunner2;                   /* 70        CS for runner on 2nd flag */
    boolean CSrunner3;                   /* 71        CS for runner on 3rd flag */
    boolean POrunner1;                   /* 72        PO for runner on 1st flag */
    boolean POrunner2;                   /* 73        PO for runner on 2nd flag */
    boolean POrunner3;                   /* 74        PO for runner on 3rd flag */
    struct evRoster* resprunner1;        /* 75        Responsible pitcher for runner on 1st */
    struct evRoster* resprunner2;        /* 76        Responsible pitcher for runner on 2nd */
    struct evRoster* resprunner3;        /* 77        Responsible pitcher for runner on 3rd */
    boolean newgameflag;                 /* 78        New Game Flag */
    boolean endgameflag;                 /* 79        End Game Flag */
    struct evRoster* pinchrunner1;       /* 80        Pinch-runner on 1st */
    struct evRoster* pinchrunner2;       /* 81        Pinch-runner on 2nd */
    struct evRoster* pinchrunner3;       /* 82        Pinch-runner on 3rd */
    struct evRoster* removedrunner1;     /* 83        Runner removed for pinch-runner on 1st */
    struct evRoster* removedrunner2;     /* 84        Runner removed for pinch-runner on 2nd */
    struct evRoster* removedrunner3;     /* 85        Runner removed for pinch-runner on 3rd */
    struct evRoster* removedbatter;      /* 86        Batter removed for pinch-hitter */
    int removedbatterposition;           /* 87        Position of batter removed for pinch-hitter */
    int putout1;                         /* 88        Fielder with First Putout (0 if none) */
    int putout2;                         /* 89        Fielder with Second Putout (0 if none) */
    int putout3;                         /* 90        Fielder with Third Putout (0 if none) */
    int assist1;                         /* 91        Fielder with First Assist (0 if none) */
    int assist2;                         /* 92        Fielder with Second Assist (0 if none) */
    int assist3;                         /* 93        Fielder with Third Assist (0 if none) */
    int assist4;                         /* 94        Fielder with Fourth Assist (0 if none) */
    int assist5;                         /* 95        Fielder with Fifth Assist (0 if none) */
    int eventnum;                        /* 96        event num */
};
void evGameInit(struct evGame* pg);
boolean evGameProcessLine(struct evGame* pg, char* str);
//void evSetHome(struct evGame* pg, TeamType h);
//void evSetStart(struct evGame* pg, struct StartSubType *s);

#endif /* _EV_GAME_H_ */