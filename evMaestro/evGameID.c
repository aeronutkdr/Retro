#include "evGameID.h"
#include <stdio.h>
#include <string.h>
#include "evPrimitives.h"

char evProcessGameID(char* str, struct evGameIDType* pid)
{
    char retval = (strlen(str) == GAMELEN);
    if (retval)
    {
        //strcpy (pid->val, str);
    }
    return retval;
}

void evDumpID(struct evGameIDType* pid)
{
    //printf ("%s\n", pid->val);
}