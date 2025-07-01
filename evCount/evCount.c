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
#define FILENAME_LENGTH (29)
/*         1         2         3
  123456789012345678901234567890
  ..\_data\YYYY\out\States.bin_ */
struct CountDataType
{
    unsigned int Freq;
    double Value;
};
static struct CountDataType CountData[3*8*4*3] = {0};
int main (int argc, char* argv[])
{
/*
    printf ("sizeof(double)              = %d\n", sizeof(double));
    printf ("sizeof(struct FilterMatch*) = %d\n", sizeof(struct FilterMatch*));
    printf ("sizeof(unsigned int)        = %d\n", sizeof(unsigned int));
    printf ("sizeof(unsigned short)      = %d\n", sizeof(unsigned short));
    printf ("sizeof(struct FilterMatch)  = %d\n", sizeof(struct FilterMatch));
*/
    char file[FILENAME_LENGTH] = "..\\_data\\";
    (void) strcat(file, argv[1]);
    (void) strcat(file, "\\out\\States.bin");
    FILE* fp = fopen (file, "rb");
    assert (fp);
    unsigned int numStates;
    assert (fread(&numStates, sizeof(unsigned int), 1, fp) == 1);
    (void) fseek(fp, 4, SEEK_CUR);
    for (unsigned int i=0; i<numStates; i++)
    {
        struct StateEntry s;
        assert (fread(&s, sizeof(struct StateEntry), 1, fp) == 1);
        int balls = (s.value>>7)&7;
        int strikes = ((s.value>>2)&0x1F) + ((s.value>>0)& 3);
        int outs = (s.value>>14) & 3;
        /* FEDCBA98 76543210 */
        /* OO321HBB BFFFFFSS */
        if (((s.value & 0x400)!=0) && (outs<3) && (balls < 4) && (strikes < 3))
        {
            unsigned int idx = ((outs)              * 8*4*3) +
                               (((s.value>>11)&0x7) *   4*3) +
                               ((balls)             *     3) +
                               strikes;
            //printf ("%04X,%d\n", s.value, idx);
            assert (idx < 288);
            CountData[idx].Freq += s.freq;
            CountData[idx].Value += (double)s.freq * (double)s.score / (double)(1<<30);
        }
    }
    (void) fclose(fp);
    for (int i=0; i<3*8*4*3; i++)
    {
        fprintf (stdout,
                 "%d,%f\n",
                 CountData[i].Freq,
                 CountData[i].Value / (double) CountData[i].Freq);
    }
    return 0;
}
/* remember to add 1 for line # */
/* 1 out, 1,3 3-1 = (1*8*4*3) + (5*4*3) + (3*3) + (1) = 96 + 60 + 9 + 1 = 166 */
/* 1 out, 1,3 3-2 = (1*8*4*3) + (5*4*3) + (3*3) + (2) = 96 + 60 + 9 + 2 = 167 */
/* 1 out,     3-1 = (1*8*4*3) + (0*4*3) + (3*3) + (1) = 96 +  0 + 9 + 1 = 106 */
/* 1 out, 1,3 3-2 = (1*8*4*3) + (0*4*3) + (3*3) + (2) = 96 +  0 + 9 + 2 = 107 */
/* 1 out,     0-0 = (1*8*4*3) + (0*4*3) + (0*3) + (0) = 96 +  0 + 0 + 0 =  96 */
/* 1 out, 1,3 0-1 = (1*8*4*3) + (0*4*3) + (3*3) + (2) = 96 +  0 + 0 + 1 =  97 */