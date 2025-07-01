#ifndef _EVPLAY_H_
#define _EVPLAY_H_
#include "evPrimitives.h"

#define FIELD_SIZE (100)
struct evPlayType
{
    int Inning;
    int Home;
    PlayerType player;
    int Count;
    char Pitches[FIELD_SIZE+1];
    char Event[FIELD_SIZE+1];
};

char evProcessPlay(char* str, struct evPlayType* ppl);
void evDumpPlay(struct evPlayType* ppl);

#endif /* _EVPLAY_H_ */
