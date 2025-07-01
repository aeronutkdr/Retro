#include "evPlay.h"
#include <stdio.h>
#include <string.h>

char evProcessPlay(char* str, struct evPlayType* ppl)
{
    memset(ppl, 0, sizeof(struct evPlayType));
    int len = strlen(str);
    char* next;
    next = memchr(str, ',' , len); *next = 0; sscanf (str, "%d", &ppl->Inning ); len -= (next-str+1); str = next+1;
    next = memchr(str, ',' , len); *next = 0; sscanf (str, "%d", &ppl->Home   ); len -= (next-str+1); str = next+1;
    next = memchr(str, ',' , len); *next = 0; sscanf (str, "%s",  ppl->player ); len -= (next-str+1); str = next+1;
    next = memchr(str, ',' , len); *next = 0; sscanf (str, "%d", &ppl->Count  ); len -= (next-str+1); str = next+1;
    next = memchr(str, ',' , len); *next = 0; sscanf (str, "%s",  ppl->Pitches); len -= (next-str+1); str = next+1;
                                              sscanf (str, "%s",  ppl->Event  ); len -= (next-str+1);
    return 6;
}

void evDumpPlay(struct evPlayType* ppl)
{
    printf ("%d\n", ppl->Inning);
    printf ("%d\n", ppl->Home);
    printf ("%s\n", ppl->player);
    printf ("%d\n", ppl->Count);
    printf ("%s\n", ppl->Pitches);
    printf ("%s\n", ppl->Event);
}