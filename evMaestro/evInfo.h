#ifndef _EVINFO_H_
#define _EVINFO_H_
#include "evPrimitives.h"

enum evInfo
{
    evi_visteam,
    evi_hometeam,
    evi_date,
    evi_site,
    evi_number,
    evi_starttime,
    evi_daynight,
    evi_usedh,
    evi_umphome,
    evi_ump1b,
    evi_ump2b,
    evi_ump3b,
    evi_pitches,
    evi_temp,
    evi_winddir,
    evi_windspeed,
    evi_fieldcond,
    evi_precip,
    evi_sky,
    evi_timeofgame,
    evi_attendance,
    evi_wp,
    evi_lp,
    evi_save,
    evi_oscorer,
    evi_unknown
};
struct evInfoType
{
    TeamType visteam;
    TeamType hometeam;
    DateType date;
    SiteType site;
    int number;
    TimeType starttime;
    char daynight;
    char usedh;
    PlayerType umphome;
    PlayerType ump1b;
    PlayerType ump2b;
    PlayerType ump3b;
    char pitches;
    int temp;
    WindType winddir;
    int windspeed;
    CondType fieldcond;
    PrecipType precip;
    SkyType sky;
    int timeofgame;
    int attendance;
    PlayerType wp;
    PlayerType lp;
    PlayerType save;
    PlayerType oscorer;
};

enum evInfo evProcessInfo(char* str, struct evInfoType* pinfo);
void evDumpInfo (struct evInfoType* pinfo);

#endif /* _EVINFO_H_ */