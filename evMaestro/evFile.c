#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "evBadj.h"
#include "evData.h"
#include "evFile.h"
#include "evGameID.h"
#include "evInfo.h"
#include "evPlay.h"
#include "evPrimitives.h"
#include "evStartSub.h"
#include "evVersion.h"
#include <stdio.h>
#include <string.h>
/* https://www.retrosheet.org/eventfile.htm */

struct evFileListType
{
    struct evFileData data;
    struct evFileListType* next;
};

struct evFileListType* evstart = 0;
struct evFileListType** evNow;
#define LINE_LEN (511)
void EVFile_Open (char* name)
{
    FILE* fp = fopen (name, "r");
    char Line[LINE_LEN+1];

    evNow = &evstart;
    struct evFileData* dNow;
    struct evInfoType     aInfo;
    struct evGameIDType   aGameID;
    struct evVersionType  aVersion;
    struct evStartSubType aStartSub;
    struct evPlayType     aPlay;
    while (fgets(Line, LINE_LEN, fp) != NULL)
    {
        //char* next = strchr(Line, ',');
        if (!memcmp(Line, ID_STR, 2))
        {
            evProcessGameID  (Line+3, &aGameID);
            *evNow = malloc (sizeof (struct evFileListType));
            dNow = &((*evNow)->data);
            strcpy (dNow->GLData.Home.Team, aGameID.HomeTeam);
            strcpy (dNow->GLData.Date, aGameID.Date);
            dNow->GLData.NumGames = aGameID.TeamGameNum;
            //evDumpID      (&aGameID);
        }
        if (!memcmp(Line, INFO_STR, 4))
        {
            switch (evProcessInfo (Line+5, &aInfo))
            {
                case evi_visteam:    strcpy(dNow->GLData.Visit.Team, aInfo.visteam);    break;
                case evi_hometeam:   strcpy(dNow->GLData.Home.Team, aInfo.hometeam);    break;
                case evi_date:       assert(!strcmp(dNow->GLData.Date, aInfo.date));    break;
                case evi_site:       strcpy(dNow->GLData.ParkID, aInfo.site);           break;
                case evi_number:     assert(dNow->GLData.NumGames == aInfo.number);     break;
                case evi_starttime:                                                     break;
                case evi_daynight:   dNow->GLData.DayNight = aInfo.daynight;            break;
                case evi_usedh:                                                         break;
                case evi_umphome:    strcpy(dNow->GLData.UmpireHome.ID, aInfo.umphome); break;
                case evi_ump1b:      strcpy(dNow->GLData.Umpire1B.ID, aInfo.ump1b);     break;
                case evi_ump2b:      strcpy(dNow->GLData.Umpire2B.ID, aInfo.ump2b);     break;
                case evi_ump3b:      strcpy(dNow->GLData.Umpire3B.ID, aInfo.ump3b);     break;
                case evi_pitches:                                                       break;
                case evi_temp:                                                          break;
                case evi_winddir:                                                       break;
                case evi_windspeed:                                                     break;
                case evi_fieldcond:                                                     break;
                case evi_precip:                                                        break;
                case evi_sky:                                                           break;
                case evi_timeofgame: dNow->GLData.TimeOfGame = aInfo.timeofgame;        break;
                case evi_attendance: dNow->GLData.Attendance = aInfo.attendance;        break;
                case evi_wp:         strcpy(dNow->GLData.WP.ID, aInfo.wp);              break;
                case evi_lp:         strcpy(dNow->GLData.LP.ID, aInfo.lp);              break;
                case evi_save:       strcpy(dNow->GLData.SaveP.ID, aInfo.save);         break;
                case evi_oscorer:                                                       break;
                case evi_unknown:                                                       break;
            }
        }
        if (!memcmp(Line, VERSION_STR, 7))
        {
            evProcessVersion (Line+8, &aVersion);
            //evDumpVersion (&aVersion);
        }
        if ((!memcmp(Line, START_STR  , 5)) ||
            (!memcmp(Line, SUB_STR    , 3)))
        {
            evProcessStartSub(Line+6, &aStartSub);
            struct GLStartType *Start = (aStartSub.Home?
                                         dNow->GLData.Home.Start:
                                         dNow->GLData.Visit.Start)+(aStartSub.Order-1);
            Start->DefensePos = aStartSub.Position;
            strcpy (Start->IDName.ID, aStartSub.player);
            //evDumpStartSub(&aStart);
        }
        if (!memcmp(Line, PLAY_STR, 4))
        {
            evProcessPlay(Line+5, &aPlay);
            switch (aPlay.Event)
            {

            }
            evDumpPlay(&aPlay);
        }
        if (!memcmp(Line, DATA_STR   , 4)) {struct evDataType     aData;    evProcessData    (Line+5, &aData);    evDumpData    (&aData);   }
        if (!memcmp(Line, BADJ_STR   , 4)) {struct evBadjType     aBadj;    evProcessBadj    (Line+5, &aBadj);    evDumpBadj    (&aBadj);   }
        if (!memcmp(Line, COM_STR    , 3));

    }
    (void) fclose(fp);
}
