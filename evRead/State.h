#ifndef STATE_H
#define STATE_H
#include <stdio.h>

/* debugging DMA */
extern int malloc_ct;
extern int malloc_sz;

/*
State0 (next: State1)  To0 (StateM) nextTo (StateN) nextTo (0)
State1 (next: State2)  To0 (StateX) nextTo (StateY) nextTo (0)
State2 (next: 0     )  To0 (0)
*/
struct ToType;
struct StateType
{
    double            score;     /* 00..07 : 8 */
    struct StateType* next;      /* 08..0B : 4 */
    struct ToType*    to0;       /* 0C..0F : 4 */
    unsigned int      freq;      /* 10..13 : 4 */
    unsigned short    value;     /* 14..15 : 2 */
    unsigned short    order;     /* 16..17 : 2 */
    unsigned short    endinning; /* 18..19 : 2 */
};
struct ToType
{
    struct StateType* st;     /* 00..03 */
    struct ToType*    nextTo; /* 04..07 */
    unsigned int      freq;   /* 08..0B */
};

/* may result in new root */
struct StateType* State_Sequence    (struct StateType* root,
                                     unsigned short*   seq,
                                     unsigned int      n,
                                     unsigned int      isFFFF);
void              State_SetOrder    (struct StateType* root);
void              State_DumpRoot    (struct StateType* root);
void              State_DumpRootBin (FILE*             fp,
                                     struct StateType* root);
void              State_UpdateScores(struct StateType* root,
                                     double*           m,
                                     unsigned int      sz);
unsigned int      State_Num         (struct StateType* root);
void              State_FreeRoot    (struct StateType* root);
unsigned int      State_Total       (struct StateType* s);
unsigned int      State_TransNum    (struct StateType* s);

#endif /* STATE_H */