#include <stdlib.h> /* for malloc, free */
#include <assert.h> /* for assert */
#include "State.h"

/* DMA management */
int malloc_ct = 0;
int malloc_sz = 0;
static void* myMalloc(int   sz) {malloc_ct++; malloc_sz+=sz; return malloc(sz);}
static void  myFree  (void* p ) {malloc_ct--;                       free  (p) ;}
#define malloc myMalloc
#define free   myFree

static void         State_Transition   (struct StateType* from,
                                        struct StateType* to);
static unsigned int State_Value        (struct StateType* s);
static unsigned int State_TransNumRoot (struct StateType* root);
static unsigned int State_TransMaxRoot (struct StateType* root);
static void         State_Dump         (struct StateType* s);
static void         State_Free         (struct StateType* s);

static void State_Transition(struct StateType* from,
                             struct StateType* to)
{
    struct ToType* t0 = 0;
    struct ToType* t = from->to0;
    while (t &&
           (t->st->value < to->value))
    {
        t0 = t;
        t  = t->nextTo;
    }
    if ((t!=0) && (t->st->value == to->value));
    else
    {
        t = malloc (sizeof (struct ToType));
        t->st     = to;
        t->nextTo = 0;
        t->freq   = 0;
        if (t0)
        {
            t->nextTo = t0->nextTo;
            t0->nextTo = t;
        }
        else
        {
            t->nextTo = from->to0;
            from->to0 = t;
        }
    }
    t->freq++;
    to->freq++;
}
static void State_Free(struct StateType* s)
{
    unsigned int numTrans = State_TransNum(s);
    struct ToType* t = s->to0;
    while (t)
    {
        struct ToType* tt = t;
        t = tt->nextTo;
        free(tt);
        numTrans--;
    }
    assert (!numTrans);
}
/* at exit: *root may represent a new root */
static struct StateType* State_Find(struct StateType** root,
                                    unsigned short v)
{
    struct StateType* prev = 0;
    struct StateType* next = *root;
    while ((next) && (next->value < v))
    {
        prev = next;
        next = next->next;
    }
    if ((!next) || (next->value != v))
    {
        next = malloc(sizeof(struct StateType));
        next->next      = 0;
        next->to0       = 0;
        next->freq      = 0;
        next->value     = v;
        next->order     = 0xFFFF;
        next->endinning = 0;
        next->score     = 0.;
        if (prev)
        {
            next->next = prev->next;
            prev->next = next;
        }
        else
        {
            prev = *root;
            *root = next;
            (*root)->next = prev;
        }
    }
    return next;
}
/* returns the root - may be different after execution */
struct StateType* State_Sequence(struct StateType* root,
                                 unsigned short* seq,
                                 unsigned int n,
                                 unsigned int isFFFF)
{
    struct StateType* from = State_Find(&root, seq[0]);
    if (isFFFF)
    {
        fprintf (stderr, "freq++ for %d\n", from->value);
        from->freq++;
    }
    for (int i=1; i<n; i++)
    {
        if ((seq[i]) == 0xFFFF)
        {
            from->endinning++;
        }
        else
        {
            struct StateType* to = State_Find(&root, seq[i]);
            State_Transition(from, to);
            from = to;
        }
    }
    return root;
}
unsigned int State_Num (struct StateType* root)
{
    unsigned int retval = 0;
    while (root)
    {
        retval++;
        root = root->next;
    }
    return retval;
}
#define LOOP_TRAP (100)
unsigned int State_TransNum (struct StateType* s)
{
    unsigned int retval = 0;
    struct ToType* to = s->to0;
    while (to && (retval < LOOP_TRAP))
    {
        retval++;
        to = to->nextTo;
    }
    assert (retval < LOOP_TRAP); /* trap infinite loop */
    return retval;
}
static unsigned int State_TransNumRoot (struct StateType* root)
{
    unsigned int retval = 0;
    while (root)
    {
        retval += State_TransNum(root);
        root = root->next;
    }
    return retval;
}
#define MAX(a,b) (((a)>(b))?(a):(b))
static unsigned int State_TransMaxRoot (struct StateType* root)
{
    unsigned int retval = 0;
    while (root)
    {
        retval = MAX(State_TransNum(root), retval);
        root = root->next;
    }
    return retval;
}
void State_SetOrder (struct StateType* root)
{
    for (unsigned short i=0; root; i++, root = root->next)
    {
        root->order = i;
    }
}
void State_FreeRoot(struct StateType* root)
{
    unsigned int numStates = State_Num(root);
    while (root)
    {
        struct StateType* next = root->next;
        State_Free(root);
        free(root);
        numStates--;
        root = next;
    }
    assert (!numStates);
}
static void State_Dump(struct StateType* s)
{
    /* FEDCBA9876543210 */
    /* OO321HBBBFFFFFSS */
    (void) fprintf (stdout, "value  = %04X %1d %1d %1d %1d %1d %1d %2d %1d\n",
                            s->value,
                            (s->value >> 14) & 0x03,
                            (s->value >> 13) & 0x01,
                            (s->value >> 12) & 0x01,
                            (s->value >> 11) & 0x01,
                            (s->value >> 10) & 0x01,
                            (s->value >>  7) & 0x07,
                            (s->value >>  2) & 0x1F,
                            (s->value >>  0) & 0x03);
    (void) fprintf (stdout, "  order  = %d\n", s->order);
    (void) fprintf (stdout, "  freq   = %d\n", s->freq);
    (void) fprintf (stdout, "  trans# = %d\n", State_TransNum(s));
    (void) fprintf (stdout, "  potent = %d\n", State_Value(s));
    (void) fprintf (stdout, "  Total  = %d\n", State_Total(s));
    (void) fprintf (stdout, "  endinn = %d\n", s->endinning);
    (void) fprintf (stdout, "  score  = %f\n", s->score);
    struct ToType* t = s->to0;
    unsigned int left = s->freq - s->endinning;
    while (t)
    {
        left -= t->freq;
        (void) fprintf (stdout, "  trans = %04X freq = %d\n",
                                (int) t->st->value,
                                t->freq);
        t = t->nextTo;
    }
    (void) fprintf (stdout, "  left#  = %d\n"  , left);
}
void State_DumpRoot(struct StateType* root)
{
    (void) fprintf (stdout, "Num States = %d\n", State_Num(root));
    (void) fprintf (stdout, "Num Trans  = %d\n", State_TransNumRoot(root));
    (void) fprintf (stdout, "Max Trans  = %d\n", State_TransMaxRoot(root));
    while (root)
    {
        State_Dump(root);
        root = root->next;
    }
}
/* FEDCBA9876543210 */
/* OO321HBBBFFFFFSS */
/*  3 outs    : 2 bit 15..14 */
/*  4 runners : 4 bit 13..10 */
/*  7 balls   : 3 bit  9.. 7 */
/* 31 fouls   : 5 bit  6.. 2 */
/*  3 strikes : 2 bit  1.. 0 */
/* total      : 16 bit */
static unsigned int State_Value (struct StateType* s)
{
    return (((s->value >> 10) & 1) +
            ((s->value >> 11) & 1) +
            ((s->value >> 12) & 1) +
            ((s->value >> 13) & 1) +
            ((s->value >> 14) & 3));
}
unsigned int State_Total (struct StateType* s)
{
    unsigned int retval = s->freq * State_Value(s);
    struct ToType* to = s->to0;
    while (to)
    {
        retval -= to->freq * State_Value(to->st);
        if (((s->value & 0x0400) == 0) &&
            ((s->value | 0x0400) == to->st->value))
        {
            retval += to->freq;
        }
        to = to->nextTo;
    }
    return retval;
}
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
static void State_DumpBin(FILE* fp, struct StateType* s)
{
    /* 12 bytes */
    fwrite(&s->value    ,sizeof(unsigned short), 1, fp);
    fwrite(&s->endinning,sizeof(unsigned short), 1, fp);
    fwrite(&s->freq     ,sizeof(unsigned int)  , 1, fp);
    double d = s->score;
    d = MIN(d, 4.);
    d = MAX(d, 0.);
    unsigned int v = (unsigned int) ((d * ((double) (1<<30))) + 0.5f);
    (void) fwrite (&v, sizeof (unsigned int), 1, fp);
}
static void State_DumpBinTrans(FILE* fp, struct StateType* s)
{
    /* 8 bytes */
    for (struct ToType* t = s->to0; t; t = t->nextTo)
    {
        fwrite(&(s->order)    ,sizeof(unsigned short), 1, fp);
        fwrite(&(t->st->order),sizeof(unsigned short), 1, fp);
        fwrite(&(t->freq)     ,sizeof(unsigned int)  , 1, fp);
    }
}
void State_DumpRootBin(FILE* fp, struct StateType* root)
{
    /* 8 bytes */
    unsigned int v = State_Num (root);
    fwrite(&v, sizeof(unsigned int), 1, fp);
    v = State_TransNumRoot(root);
    fwrite(&v, sizeof(unsigned int), 1, fp);
    for (struct StateType* s = root; s; s=s->next)
    {
        State_DumpBin(fp, s);
    }
    for (struct StateType* s = root; s; s=s->next)
    {
        State_DumpBinTrans(fp, s);
    }
}
void State_UpdateScores(struct StateType* root,
                        double*           m,
                        unsigned int      sz)
{
    int i;
    struct StateType* s;
    for (i=0, s = root; s; s=s->next, i+=(sz+1))
    {
        double* p = m+i;
        s->score = p[sz]/p[s->order];
    }
}