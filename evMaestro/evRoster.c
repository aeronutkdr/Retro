#include "evRoster.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct evRosterNode
{
    PlayerType           ID;
    struct evRosterType  Data;
    struct evRosterNode* Next;
};
struct evRosterNode* evRoster = 0;

static struct evRosterNode* evRoster_Create(char* str)
{
    struct evRosterNode* retval = (struct evRosterNode*) malloc (sizeof(struct evRosterNode));
    memset(retval, 0, sizeof(struct evRosterNode));
    int len = strlen(str);
    char* next;
    next = memchr(str, ',' , len); *next = 0; sscanf (str, "%s", retval->ID);                                       len -= (next-str+1); str = next+1;
    next = memchr(str, ',' , len); *next = 0; sscanf (str, "%s", retval->Data.LastName);                            len -= (next-str+1); str = next+1;
    next = memchr(str, ',' , len); *next = 0; sscanf (str, "%s", retval->Data.FirstName);                           len -= (next-str+1); str = next+1;
    next = memchr(str, ',' , len); *next = 0; retval->Data.BatL = (*str != 'R'); retval->Data.BatR = (*str != 'L'); len -= (next-str+1); str = next+1;
    next = memchr(str, ',' , len); *next = 0; retval->Data.ThrowR = (*str == 'R');                                  len -= (next-str+1); str = next+1;
    next = memchr(str, ',' , len); *next = 0; sscanf (str, "%s", retval->Data.Team);                                len -= (next-str+1); str = next+1;
                                              sscanf (str, "%s", retval->Data.Position);                            len -= (next-str+1);
    retval->Next = 0;
    return retval;
}

void evRoster_Add(char* str)
{
    struct evRosterNode *p = evRoster_Create(str);
    if (!evRoster)
    {
        evRoster = p;
    }
    else if (strcmp(p->ID, evRoster->ID)<0)
    {
        p->Next = evRoster;
        evRoster = p;
    }
    else
    {
        struct evRosterNode *prev = evRoster;
        struct evRosterNode* n;
        for (n = prev->Next;
             n && strcmp(p->ID, prev->ID)>=0;
             prev = n, n = n->Next);
        if (strcmp(p->ID, prev->ID))
        {
            p->Next = n;
            prev->Next = p;
        }
    }
}

struct evRosterType* evRoster_Get(PlayerType id)
{
    struct evRosterNode* retval = evRoster;
    while (retval && strcmp (id, retval->ID) > 0) retval = retval->Next;
    if (!retval) return 0;
    return &retval->Data;
}

void evRoster_Free(void)
{
    while (evRoster)
    {
        struct evRosterNode* n = evRoster->Next;
        free(evRoster);
        evRoster = n;
    }
}

void evRoster_Dump(struct evRosterType* p)
{
    if (p)
    {
        printf ("%s\n", p->LastName);
        printf ("%s\n", p->FirstName);
        printf ("%s\n", p->BatL?"true":"false");
        printf ("%s\n", p->BatR?"true":"false");
        printf ("%s\n", p->ThrowR?"true":"false");
        printf ("%s\n", p->Team);
        printf ("%s\n", p->Position);
    }
}

void evRoster_DumpList(void)
{
    for (struct evRosterNode* n = evRoster; n; n = n->Next)
    {
        printf ("%s,%s\n", n->ID,n->Data.Team);
    }
}