#include "RetroBinFile.h"
#include "mystdlib.h"
#include <assert.h>
#include <stdio.h>

/* 
n states (uint16)
pad (uint16)
State_0, NumTos_0 (uint16   uint16)
State_1, NumTos_1 (uint16   uint16)
State_2, NumTos_2 (uint16   uint16)
State_., NumTos_. (uint16   uint16)
State_., NumTos_. (uint16   uint16)
State_n-1, NumTos_n-1 (uint16   uint16)
State0Val (uint32)
State0Enter(uint32)
:w

*/

struct ToType
{
	unsigned short State;
	unsigned short Pad;
	unsigned int   Freq;
};
struct StateType
{
	unsigned short State;
	unsigned short NumTos;
	unsigned int   Value;
	unsigned int   FreqIn;
};

static unsigned char* fbin = 0;
static long           flen = 0;
void RBFile_Open(char* fname)
{
    FILE* fp = fopen (fname, "rb");
    fseek(fp, 0, SEEK_END);
    flen = ftell(fp);
    fbin = malloc(flen);
    fseek(fp, 0, SEEK_SET);
    fread(fbin, 1, flen, fp);
    fclose(fp);
    unsigned char* p=fbin;
    struct StateType* stateptrs[0x4000] = {0};
    while (p < fbin+flen)
    {
        assert (((int)p&3)==0);
        struct StateType* s = (struct StateType*) p;
        stateptrs[s->State] = s;
        p += s->NumTos*sizeof(struct ToType) + sizeof(struct StateType);
    }
    p = fbin;
    while (p < fbin+flen)
    {
        assert (((int)p&3)==0);
        struct StateType* s = (struct StateType*) p;
        for (int i=0; i<s->NumTos; i++)
        {
            struct ToType* t = (struct ToType*) (p+i*sizeof(struct ToType) + sizeof(struct StateType));
            unsigned int tostate = t->State;
            t->State = stateptrs[tostate];
        }
        p += s->NumTos*sizeof(struct ToType) + sizeof(struct StateType);
    }
}
void RBFile_Close(void)
{
    if (fbin)
    {
        free(fbin);
        fbin = 0;
        flen = 0;
    }
}

struct State* RBFile_GetState(unsigned short s)
{
    unsigned char found = 0;
    unsigned char* p = fbin;
    struct State* st;
    while (p < (fbin+flen))
    {
        st = (struct State*) p;
        found = st->State == s;
        if (found) break;
        unsigned short n = st->NumTos;
        p += sizeof(struct State) + n*sizeof(struct To);
    }
    if (found) return st;
    else return 0;
}

struct State* RBFile_GetStateByIndex(int i)
{
    unsigned char* p = fbin;
    struct State* st;
    while (i && (p < (fbin+flen)))
    {
        i--;
        st = (struct State*) p;
        unsigned short n = st->NumTos;
        p += sizeof(struct State) + n*sizeof(struct To);
    }
    if ((p < (fbin+flen))) return st;
    else return 0;
}
struct To* RBFile_GetNext(struct State* s, int i)
{
    if (i>s->NumTos) return 0;
    return (struct To*) ((unsigned char*) s + sizeof(struct State) + i*sizeof(struct To));
}