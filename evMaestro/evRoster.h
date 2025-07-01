#ifndef _EVROSTER_H_
#define _EVROSTER_H_
#include "evPrimitives.h"

#define ROSTER_NAME_SIZE (20)
#define ROSTER_POS_SIZE (3)
struct evRosterType
{
    char LastName[ROSTER_NAME_SIZE];
    char FirstName[ROSTER_NAME_SIZE];
    char BatR;
    char BatL;
    char ThrowR;
    TeamType Team;
    char Position[ROSTER_POS_SIZE];
};

void evRoster_Add(char* str);
struct evRosterType* evRoster_Get(PlayerType id);
void evRoster_Free(void);

void evRoster_Dump(struct evRosterType* p);
void evRoster_DumpList(void);

#endif /* _EVROSTER_H_ */
