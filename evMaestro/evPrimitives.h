#ifndef _EVPRIMITIVES_H_
#define _EVPRIMITIVES_H_

enum LineType
{
    id,
    version,
    info,
    start,
    play,
    sub,
    data,
    com
};

#define GAMELEN (12)
#define TEAMLEN (3)
#define DATELEN (10)
#define SITELEN (5)
#define TIMELEN (6)
#define PLAYERLEN (8)
#define WINDLEN (7)   /* : fromcf, fromlf, fromrf, ltor, rtol, tocf, tolf, torf, unknown */
#define CONDLEN (7)   /* : dry, soaked, wet, unknown */
#define PRECIPLEN (7) /* : drizzle, none, rain, showers, snow, unknown */
#define SKYLEN (8)    /* : cloudy, dome, night, overcast, sunny, unknown */

typedef char GameType[GAMELEN+1];
typedef char TeamType[TEAMLEN+1];
typedef char DateType[DATELEN+1];
typedef char SiteType[SITELEN+1];
typedef char TimeType[TIMELEN+1];
typedef char PlayerType[PLAYERLEN+1];
typedef char WindType[WINDLEN+1];
typedef char CondType[CONDLEN+1];
typedef char PrecipType[PRECIPLEN+1];
typedef char SkyType[SKYLEN+1];

#define ID_STR "id"
#define VERSION_STR "version"
#define INFO_STR "info"
#define START_STR "start"
#define PLAY_STR "play"
#define SUB_STR "sub"
#define DATA_STR "data"
#define COM_STR "com"
#define BADJ_STR "badj"

#endif /* _EVPRIMITIVES_H_ */