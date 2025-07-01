#include "evVersion.h"
#include <stdio.h>

char evProcessVersion(char* str, struct evVersionType* pversion)
{
    return sscanf (str, "%d", &pversion->val);
}

void evDumpVersion(struct evVersionType* pversion)
{
    printf ("%d\n", pversion->val);
}