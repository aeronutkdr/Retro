#include "evData.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

char evProcessData(char* str, struct evDataType* pdat)
{
    char ty[100];
    int len = strlen(str);
    char* next;
    next = memchr(str, ',' , len); *next = 0; sscanf (str, "%s",  ty          ); len -= (next-str+1); str = next+1;
    next = memchr(str, ',' , len); *next = 0; sscanf (str, "%s",  pdat->player); len -= (next-str+1); str = next+1;
                                              sscanf (str, "%d", &pdat->Earned);
    assert (strcmp ("er", ty) == 0);
    return 2;
}

void evDumpData(struct evDataType* pdat)
{
    printf ("%s\n", pdat->player);
    printf ("%d\n", pdat->Earned);
}
