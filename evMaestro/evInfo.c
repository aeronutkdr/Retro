#include "evInfo.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

enum evInfo evProcessInfo(char* str, struct evInfoType* pinfo)
{
    enum evInfo retval = evi_unknown;
	char* tok = strtok(str, ",");
    char* val = strtok(NULL, ",");
    if (!strcmp(tok, "visteam"   )) retval = evi_visteam;
    if (!strcmp(tok, "hometeam"  )) retval = evi_hometeam;
    if (!strcmp(tok, "date"      )) retval = evi_date;
    if (!strcmp(tok, "site"      )) retval = evi_site;
    if (!strcmp(tok, "number"    )) retval = evi_number;
    if (!strcmp(tok, "starttime" )) retval = evi_starttime;
    if (!strcmp(tok, "daynight"  )) retval = evi_daynight;
    if (!strcmp(tok, "usedh"     )) retval = evi_usedh;
    if (!strcmp(tok, "umphome"   )) retval = evi_umphome;
    if (!strcmp(tok, "ump1b"     )) retval = evi_ump1b;
    if (!strcmp(tok, "ump2b"     )) retval = evi_ump2b;
    if (!strcmp(tok, "ump3b"     )) retval = evi_ump3b;
    if (!strcmp(tok, "pitches"   )) retval = evi_pitches;
    if (!strcmp(tok, "temp"      )) retval = evi_temp;
    if (!strcmp(tok, "winddir"   )) retval = evi_winddir;
    if (!strcmp(tok, "windspeed" )) retval = evi_windspeed;
    if (!strcmp(tok, "fieldcond" )) retval = evi_fieldcond;
    if (!strcmp(tok, "precip"    )) retval = evi_precip;
    if (!strcmp(tok, "sky"       )) retval = evi_sky;
    if (!strcmp(tok, "timeofgame")) retval = evi_timeofgame;
    if (!strcmp(tok, "attendance")) retval = evi_attendance;
    if (!strcmp(tok, "wp"        )) retval = evi_wp;
    if (!strcmp(tok, "lp"        )) retval = evi_lp;
    if (!strcmp(tok, "save"      )) retval = evi_save;
    if (!strcmp(tok, "oscorer"   )) retval = evi_oscorer;

    switch (retval)
    {
        case evi_visteam:    strcpy(pinfo->visteam  , val);            break;
        case evi_hometeam:   strcpy(pinfo->hometeam , val);            break;
        case evi_date:       strcpy(pinfo->date     , val);            break;
        case evi_site:       strcpy(pinfo->site     , val);            break;
        case evi_number:     sscanf(val, "%d", &pinfo->number);        break;
        case evi_starttime:  strcpy(pinfo->starttime, val);            break;
        case evi_daynight:   pinfo->daynight = strcmp(val, "day");     break;
        case evi_usedh:      pinfo->usedh = 1;                         break;
        case evi_umphome:    strcpy(pinfo->umphome  , val);            break;
        case evi_ump1b:      strcpy(pinfo->ump1b    , val);            break;
        case evi_ump2b:      strcpy(pinfo->ump2b    , val);            break;
        case evi_ump3b:      strcpy(pinfo->ump3b    , val);            break;
        case evi_pitches:    pinfo->pitches = strcmp (val, "pitches"); break;
        case evi_temp:       sscanf(val, "%d", &pinfo->temp);          break;
        case evi_winddir:    strcpy(pinfo->winddir  , val);            break;
        case evi_windspeed:  sscanf(val, "%d", &pinfo->windspeed);     break;
        case evi_fieldcond:  strcpy(pinfo->fieldcond, val);            break;
        case evi_precip:     strcpy(pinfo->precip   , val);            break;
        case evi_sky:        strcpy(pinfo->sky      , val);            break;
        case evi_timeofgame: sscanf(val, "%d", &pinfo->timeofgame);    break;
        case evi_attendance: sscanf(val, "%d", &pinfo->attendance);    break;
        case evi_wp:         strcpy(pinfo->wp       , val);            break;
        case evi_lp:         strcpy(pinfo->lp       , val);            break;
        case evi_save:       strcpy(pinfo->save     , val);            break;
        case evi_oscorer:    strcpy(pinfo->oscorer  , val);            break;
        case evi_unknown:                                              break;
    }
    return retval;
}

void evDumpInfo(struct evInfoType* pinfo)
{
    printf ("%s\n", pinfo->visteam);
    printf ("%s\n", pinfo->hometeam);
    printf ("%s\n", pinfo->date);
    printf ("%s\n", pinfo->site);
    printf ("%d\n", pinfo->number);
    printf ("%s\n", pinfo->starttime);
    printf ("%c\n", pinfo->daynight);
    printf ("%c\n", pinfo->usedh);
    printf ("%s\n", pinfo->umphome);
    printf ("%s\n", pinfo->ump1b);
    printf ("%s\n", pinfo->ump2b);
    printf ("%s\n", pinfo->ump3b);
    printf ("%c\n", pinfo->pitches);
    printf ("%d\n", pinfo->temp);
    printf ("%s\n", pinfo->winddir);
    printf ("%d\n", pinfo->windspeed);
    printf ("%s\n", pinfo->fieldcond);
    printf ("%s\n", pinfo->precip);
    printf ("%s\n", pinfo->sky);
    printf ("%d\n", pinfo->timeofgame);
    printf ("%d\n", pinfo->attendance);
    printf ("%s\n", pinfo->wp);
    printf ("%s\n", pinfo->lp);
    printf ("%s\n", pinfo->save);
}