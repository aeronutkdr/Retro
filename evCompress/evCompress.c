#include <stdio.h>
#include <string.h>
#include <assert.h>
struct StateEntry
{
    unsigned short value;
    unsigned short endinning;
    unsigned int   freq;
    unsigned int   score;
};
/* FEDC_BA98_7654_3210 */
/* OO32_1HBB_BFFF_FFSS */
#define NUM_CONTRIB     ( 16)
#define LINE_LEN        (NUM_CONTRIB*9+6)
static double Values[65536];
int main (int argc, char* argv[])
{
                /* 01\234567\89012\3456\789012345678 */
    char file[] = "..\\_data\\YYYY\\out\\States.bin";
    (void) memcpy(file+9, argv[1], 4);
    FILE* fp = fopen (file, "rb");
    assert (fp);
    unsigned int numStates;
    assert (fread(&numStates, sizeof(unsigned int), 1, fp) == 1);
    (void) fseek(fp, 4, SEEK_CUR);
    (void) memset(Values, 0xFF, 65536*sizeof(double));
    for (int i=0; i<numStates; i++)
    {
        struct StateEntry s;
        assert (fread(&s, sizeof(struct StateEntry), 1, fp) == 1);
        Values[s.value] = ((double)s.score)/((double)(1<<30));
        int outs = (s.value & 0xC000) >> 14;
        int runners = ((s.value & 0x2000)?1:0) +
                      ((s.value & 0x1000)?1:0) +
                      ((s.value & 0x0800)?1:0) +
                      ((s.value & 0x0400)?1:0);
        Values[s.value] -= (double)outs;
        Values[s.value] -= (double)runners;
    }
    (void) fclose(fp);
    fp = fopen (argv[2], "r");
    char line[LINE_LEN+1];
    (void) fgets(line, LINE_LEN, fp);
    int stLast;
    assert (sscanf(line, "%X", &stLast) == 1);
    int st0 = stLast;
    assert (fprintf (stdout, "%04X,%lf\n", st0, Values[st0]) > 0);
    assert (strspn((const char*) (Values+st0), "\xFF") < sizeof(double));
    double Contrib[NUM_CONTRIB] = {0.};
    double Weight[NUM_CONTRIB] = {0.};
    while (fgets(line, LINE_LEN, fp) != NULL)
    {
        unsigned int pos[NUM_CONTRIB];
        int st;
#if (NUM_CONTRIB != 16)
#error (NUM_CONTRIB != 16)
#endif /* (NUM_CONTRIB != 16) */
        assert (sscanf(line,
                       "%X%*c%X%*c%X%*c%X%*c%X%*c%X%*c%X%*c%X%*c%X%*c%X%*c%X%*c%X%*c%X%*c%X%*c%X%*c%X%*c%X",
                       &st,
                       pos+ 0, pos+ 1, pos+ 2, pos+ 3,
                       pos+ 4, pos+ 5, pos+ 6, pos+ 7,
                       pos+ 8, pos+ 9, pos+10, pos+11,
                       pos+12, pos+13, pos+14, pos+15) == (NUM_CONTRIB+1));
        unsigned int sum=1;
        assert (strspn((const char*)(Values+st), "\xFF") < sizeof(double));
        double d_st = (Values[st] - Values[stLast])/((double)0xFFFFFFFF);
        assert (fprintf (stdout, "%04X,%lf\n", st, Values[st]) > 0);
        for (int i=0; i<NUM_CONTRIB; i++)
        {
            Contrib[i] += (d_st * ((double)pos[i]));
            Weight[i]  += (double)pos[i];
            sum        += pos[i];
        }
        assert (!sum);
        stLast = st;
    }
    (void) fclose(fp);
    assert (fprintf (stdout, "%04X %04X\n", st0, stLast) > 0);
    double d_st = 1./(Values[stLast] - Values[st0]);
    assert (fprintf (stdout, "%lf\n", d_st) > 0);
    for (int i=0; i<NUM_CONTRIB; i++)
    {
        assert (fprintf (stdout,
                         "%lf,%lf,%lf\n",
                         Weight[i]/((double)0xFFFFFFFF),Contrib[i],Contrib[i]*d_st) > 0);
    }
    return 0;
}