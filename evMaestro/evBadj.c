#include "evBadj.h"
#include <stdio.h>
#include <string.h>

char evProcessBadj(char* str, struct evBadjType* pbadj)
{
    int len = strlen(str);
    char* next;
    char Left;
    next = memchr(str, ',' , len); *next = 0; sscanf (str, "%s",  pbadj->player); len -= (next-str+1); str = next+1;
                                              sscanf (str, "%c", &Left);          len -= (next-str+1);
    pbadj->LeftHand = (Left == 'L');
    return 2;
}

void evDumpBadj(struct evBadjType* pbadj)
{
    printf ("%s\n", pbadj->player);
    printf ("%d\n", pbadj->LeftHand);
}
