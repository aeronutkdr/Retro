#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
/* DMA management */
int malloc_ct = 0;
int malloc_sz = 0;
static void* myMalloc(int   sz) {malloc_ct++; malloc_sz+=sz; return malloc(sz);}
static void  myFree  (void* p ) {malloc_ct--;                       free  (p) ;}
#define malloc myMalloc
#define free   myFree
#define DEBUG_PRINT() (void) fprintf (stdout, "**** %d %d ****\n",\
                                      malloc_ct, malloc_sz);
struct FilterMatch
{
    double              val;
    struct FilterMatch* next;
    unsigned int        freq;
    unsigned short      filterVal;
    unsigned short      numMatch;
};
struct StateEntry
{
    unsigned short value;
    unsigned short endinning;
    unsigned int   freq;
    unsigned int   score;
};
struct FilterMatch* FilterFind (struct FilterMatch* root, unsigned short f)
{
    struct FilterMatch* retval = 0;
    struct FilterMatch* prev = 0;
    for (retval = root; retval; retval = retval->next)
    {
        if (retval->filterVal == f) break;
        prev = retval;
    }
    if (!retval)
    {
        retval = malloc(sizeof(struct FilterMatch));
        retval->filterVal = f;
        retval->freq = 0;
        retval->next = 0;
        retval->val  = 0.;
        retval->numMatch = 0;
        if (prev)
        {
            retval->next = prev->next;
            prev->next   = retval;
        }
    }
    return retval;
}
struct FilterMatch* FilterAdd(struct FilterMatch* root,
                              struct StateEntry*  s,
                              unsigned short      f)
{
    f &= s->value;
    struct FilterMatch* fm = FilterFind(root, f);
    fm->freq += s->freq;
    fm->val += ((double) s->freq * (double) s->score) / (double) (1<<30);
    fm->numMatch++;
    if (!root) return fm;
    else return root;
}
void FilterFree(struct FilterMatch* root)
{
    struct FilterMatch* n;
    for (struct FilterMatch* f = root; f; f = n)
    {
        n = f->next;
        free(f);
    }
}
#define MIN(a,b) (((a)<(b))?(a):(b))
void FilterStateDump(char* str, unsigned short s)
{
    /* "2O 321H 3-2"  */
    /* 12345678901234 */
    sprintf (str,
             "\"%dO %c%c%c%c %d-%d\"",
             ((s>>14)&0x3),
             ((s&0x2000)?'3':' '),
             ((s&0x1000)?'2':' '),
             ((s&0x0800)?'1':' '),
             ((s&0x0400)?'H':' '),
             MIN(((s>>7)&0x07),3),
             MIN((((s>>2)&0x1F)+((s>>0)&0x03)),2));
}
void FilterDump(struct FilterMatch* root)
{
    char State[14];
    for (;root;root = root->next)
    {
        FilterStateDump(State, root->filterVal);
        printf ("%04X,%s,%d,%d,%f\n",
                root->filterVal,
                State,
                root->numMatch,
                root->freq,
                root->val / (double) root->freq);
    }
}
#define FILENAME_LENGTH (29)
/*         1         2         3
  123456789012345678901234567890
  ..\_data\YYYY\out\States.bin_ */
int main (int argc, char* argv[])
{
/*
    printf ("sizeof(double)              = %d\n", sizeof(double));
    printf ("sizeof(struct FilterMatch*) = %d\n", sizeof(struct FilterMatch*));
    printf ("sizeof(unsigned int)        = %d\n", sizeof(unsigned int));
    printf ("sizeof(unsigned short)      = %d\n", sizeof(unsigned short));
    printf ("sizeof(struct FilterMatch)  = %d\n", sizeof(struct FilterMatch));
*/
    struct FilterMatch* root = 0;
    unsigned short filterMask, filterMatch, groupBy;
    int ival;
    DEBUG_PRINT();
    assert (sscanf (argv[2], "%X", &ival) == 1); filterMask = ival;
    assert (sscanf (argv[3], "%X", &ival) == 1); filterMatch = ival;
    assert (sscanf (argv[4], "%X", &ival) == 1); groupBy = ival;
    char file[FILENAME_LENGTH] = "..\\_data\\";
    (void) strcat(file, argv[1]);
    (void) strcat(file, "\\out\\States.bin");
    FILE* fp = fopen (file, "rb");
    assert (fp);
    unsigned int numStates;
    assert (fread(&numStates, sizeof(unsigned int), 1, fp) == 1);
    (void) fseek(fp, 4, SEEK_CUR);
    unsigned int RejectCount = 0;
    for (unsigned int i=0; i<numStates; i++)
    {
        struct StateEntry s;
        assert (fread(&s, sizeof(struct StateEntry), 1, fp) == 1);
        if ((s.value & filterMask) == (filterMatch))
            root = FilterAdd(root, &s, groupBy);
        else
            RejectCount++;
    }
    (void) fclose(fp);
    FilterDump(root);
    (void) printf ("Reject Count = %d\n", RejectCount);
    DEBUG_PRINT();
    FilterFree(root);
    DEBUG_PRINT();
    return 0;
}