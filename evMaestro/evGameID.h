#ifndef _EVGAMEID_H_
#define _EVGAMEID_H_
//#include "evPrimitives.h"
#include "glFileTypes.h"

struct evGameIDType
{
    //GameType val;
    char HomeTeam[TEAM_SIZE];
    char Date[DATE_SIZE];
    unsigned char TeamGameNum;
};

char evProcessGameID(char* str, struct evGameIDType* pid);
void evDumpID(struct evGameIDType* pid);

#endif /* _EVGAMEID_H_ */