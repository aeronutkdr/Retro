#include "evStartSub.h"
#include <stdio.h>
#include <string.h>

char evProcessStartSub(char* str, struct evStartSubType* pss)
{
    int len = strlen(str);
    char* next;
    next = memchr(str, ',' , len); *next = 0; sscanf (str, "%s",  pss->player  ); len -= (next-str+1); str = next+1;
    next = memchr(str, ',' , len); *next = 0; strcpy (pss->Fullname, str+1);      len -= (next-str+1); str = next+1;
    next = memchr(str, ',' , len); *next = 0; sscanf (str, "%d", &pss->Home    ); len -= (next-str+1); str = next+1;
    next = memchr(str, ',' , len); *next = 0; sscanf (str, "%d", &pss->Order   ); len -= (next-str+1); str = next+1;
                                              sscanf (str, "%d", &pss->Position); len -= (next-str+1);
    * (char*)memchr(pss->Fullname, '"', NAME_SIZE) = 0;
    return 5;
}

void evDumpStartSub(struct evStartSubType* pss)
{
    printf ("%s\n", pss->player);
    printf ("%s\n", pss->Fullname);
    printf ("%d\n", pss->Home);
    printf ("%d\n", pss->Order);
    printf ("%d\n", pss->Position);
}