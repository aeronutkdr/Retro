#ifndef _GLFILETYPES_H_
#define _GLFILETYPES_H_
/* https://www.retrosheet.org/gamelogs/glfields.txt */

#define DATE_SIZE    (8+1)
#define PARK_SIZE    (5+1)
#define ID_SIZE      (8+1)
#define DOW_SIZE     (3+1)
#define TEAM_SIZE    (3+1)
#define LEAGUE_SIZE  (2+1)
#define PROTEST_SIZE (2+1)
#define NUM_STARTS   (9)
#define LS_SIZE      (50+1)
struct GLCompletion
{
   /* this field will include:
      "yyyymmdd,park,vs,hs,len" Where */
   char          Date[DATE_SIZE]; /* yyyymmdd -- the date the game was completed */
   char          Park[PARK_SIZE]; /* park -- the park ID where the game was completed */
   unsigned char vs;              /* vs -- the visitor score at the time of interruption */
   unsigned char hs;              /* hs -- the home score at the time of interruption */
   unsigned char len;             /* len -- the length of the game in outs at time of interruption */
   /* All the rest of the information in the record refers to the entire game. */
};
struct GLOffenseStats
{
   unsigned char AB;  /* at-bats */
   unsigned char H;   /* hits */
   unsigned char B2;  /* doubles */
   unsigned char B3;  /* triples */
   unsigned char HR;  /* homeruns */
   unsigned char RBI; /* RBI */
   unsigned char SH;  /* sacrifice hits.  This may include sacrifice flies for years
                           prior to 1954 when sacrifice flies were allowed. */
   unsigned char SF;  /* sacrifice flies (since 1954) */
   unsigned char HBP; /* hit-by-pitch */
   unsigned char BB;  /* walks */
   unsigned char IBB; /* intentional walks */
   unsigned char K;   /* strikeouts */
   unsigned char SB;  /* stolen bases */
   unsigned char CS;  /* caught stealing */
   unsigned char GDP; /* grounded into double plays */
   unsigned char CI;  /* awarded first on catcher's interference */
   unsigned char LOB; /* left on base */
};
struct GLPitchStats
{
   unsigned char PitchersUsed; /* pitchers used ( 1 means it was a complete game ) */
   unsigned char PitchERs;     /* individual earned runs */
   unsigned char TeamERs;      /* team earned runs */
   unsigned char WP;           /* wild pitches */
   unsigned char BK;           /* balks */
};
struct GLDefenseStats
{
   unsigned char Putouts; /* putouts.  Note: prior to 1931, this may not equal 3 times
                                          the number of innings pitched.  Prior to that, no
                                          putout was awarded when a runner was declared out for
                                          being hit by a batted ball. */
   unsigned char Assists; /* assists */
   unsigned char E;       /* errors */
   unsigned char PB;      /* passed balls */
   unsigned char DP;      /* double plays */
   unsigned char TP;      /* triple plays */
};
struct GLIDNameType
{
   char ID[ID_SIZE];
   //char *Name;
};
struct GLStartType
{
   struct GLIDNameType IDName;
   unsigned char       DefensePos;
};

struct GLTeamDataType
{
   char Team[TEAM_SIZE];
   char League[LEAGUE_SIZE];             /*   4-  5,  7-  8 team and league */
   unsigned char TeamGameNum;            /*   6    ,  9     team game number
                                                              For this and the home team game number, ties are counted as
                                                              games and suspended games are counted from the starting
                                                              rather than the ending date. */
   unsigned char TeamScore;              /*  10    , 11     team score (unquoted) */
   char LineScore[LS_SIZE];              /*  20    , 21     line scores.  For example:
                                                              "010000(10)0x"
                                                              Would indicate a game where the home team scored a run in
                                                              the second inning, ten in the seventh and didn't bat in the
                                                              bottom of the ninth. */
   struct GLOffenseStats OffenseStats;   /*  22- 38, 50- 66 team offensive statistics (unquoted) (in order) */
   struct GLPitchStats PitchStats;       /*  39- 43, 67- 71 team pitching statistics (unquoted)(in order): */
   struct GLDefenseStats DefenseStats;   /*  44- 49, 72- 77 team defensive statistics (unquoted) (in order): */
   struct GLIDNameType Manager;          /*  90- 91, 92- 93 team manager ID and name */
   struct GLIDNameType SP;               /* 102-103,104-105 starting pitcher ID and name */
   struct GLStartType Start[NUM_STARTS]; /* 106-132,133-159 starting players ID, name and defensive position,
                                                              listed in the order (1-9) they appeared in the batting order. */
};

struct GLFileType
{
   char Date[DATE_SIZE];                    /* 1     Date in the form "yyyymmdd" */
   char NumGames;                           /* 2     Number of game:
                                                      "0" -- a single game
                                                      "1" -- the first game of a double (or triple) header
                                                            including seperate admission doubleheaders
                                                      "2" -- the second game of a double (or triple) header
                                                            including seperate admission doubleheaders
                                                      "3" -- the third game of a triple-header
                                                      "A" -- the first game of a double-header involving 3 teams
                                                      "B" -- the second game of a double-header involving 3 teams */
   char DayOfWeek[DOW_SIZE];                /* 3     Day of week  ("Sun","Mon","Tue","Wed","Thu","Fri","Sat") */
   unsigned char NumOuts;                   /* 12     Length of game in outs (unquoted).  A full 9-inning game would
                                                      have a 54 in this field.  If the home team won without batting
                                                      in the bottom of the ninth, this field would contain a 51. */
   char DayNight;                           /* 13     Day/night indicator ("D" or "N") */
   struct GLCompletion Completion;          /* 14     Completion information.  If the game was completed at a
                                                      later date (either due to a suspension or an upheld protest) */
   char ForfeitInfo;                        /* 15     Forfeit information:
                                                      "V" -- the game was forfeited to the visiting team
                                                      "H" -- the game was forfeited to the home team
                                                      "T" -- the game was ruled a no-decision */
   char ProtestInfo[PROTEST_SIZE];          /* 16     Protest information:
                                                         "P" -- the game was protested by an unidentified team
                                                         "V" -- a disallowed protest was made by the visiting team
                                                         "H" -- a disallowed protest was made by the home team
                                                         "X" -- an upheld protest was made by the visiting team
                                                         "Y" -- an upheld protest was made by the home team
                                                      Note: two of these last four codes can appear in the field
                                                      (if both teams protested the game). */
   char ParkID[PARK_SIZE];                  /* 17     Park ID */
   unsigned int Attendance;                 /* 18     Attendance (unquoted) */
   unsigned int TimeOfGame;                 /* 19     Time of game in minutes (unquoted) */
   struct GLIDNameType UmpireHome;          /* 78-79     Home plate umpire ID and name */
   struct GLIDNameType Umpire1B;            /* 80-81     1B umpire ID and name */
   struct GLIDNameType Umpire2B;            /* 82-83     2B umpire ID and name */
   struct GLIDNameType Umpire3B;            /* 84-85     3B umpire ID and name */
   struct GLIDNameType UmpireLF;            /* 86-87     LF umpire ID and name */
   struct GLIDNameType UmpireRF;            /* 88-89     RF umpire ID and name */
                                               /* If any umpire positions were not filled for a particular game
                                                  the fields will be "","(none)". */
   struct GLIDNameType WP;                  /* 94-95     Winning pitcher ID and name */
   struct GLIDNameType LP;                  /* 96-97     Losing pitcher ID and name */
   struct GLIDNameType SaveP;               /* 98-99     Saving pitcher ID and name--"","(none)" if none awarded */
   struct GLIDNameType GWRBI;               /* 100-101   Game Winning RBI batter ID and name--"","(none)" if none awarded */
   char* AdditionalInfo;                    /* 160       Additional information.  This is a grab-bag of informational
                                                         items that might not warrant a field on their own.  The field 
                                                         is alpha-numeric. Some items are represented by tokens such as:
                                                            "HTBF" -- home team batted first.
                                                            Note: if "HTBF" is specified it would be possible to see
                                                            something like "01002000x" in the visitor's line score.
                                                         Changes in umpire positions during a game will also appear in 
                                                         this field.  These will be in the form:
                                                            umpchange,inning,umpPosition,umpid with the latter three
                                                            repeated for each umpire.
                                                         These changes occur with umpire injuries, late arrival of 
                                                         umpires or changes from completion of suspended games. Details
                                                         of suspended games are in field 14. */
   char AcquisitionInfo;                    /* 161       Acquisition information:
                                                         "Y" -- we have the complete game
                                                         "N" -- we don't have any portion of the game
                                                         "D" -- the game was derived from box score and game story
                                                         "P" -- we have some portion of the game.  We may be missing
                                                               innings at the beginning, middle and end of the game. */
   struct GLTeamDataType Visit;
   struct GLTeamDataType Home;
   /* Missing fields will be NULL. */
};

#endif /* _GLFILETYPES_H_ */