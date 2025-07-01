#include <stdio.h>
#include <string.h>

int RunValue(int v)
{
    v >>= 8;
    int retval = (v >> 4);
    retval += (v&1)?1:0;
    retval += (v&2)?1:0;
    retval += (v&4)?1:0;
    retval += (v&8)?1:0;
    return retval;
}

#define LINE_SIZE (1000)
int main (int argc, char* argv[])
{
    FILE* fp = fopen(argv[1], "r");
    char Line[LINE_SIZE + 1];
    int sumruns = 0;
    while (fgets(Line, LINE_SIZE, fp) != NULL)
    {
        if (strncmp("val = ", Line, 6)) continue;
        char* from_str = strchr(Line, '=') + 2;
        char* num_str = strchr(Line, ',') + 1;
        int from, num;
        sscanf(from_str, "%x", &from);
        if ((from & 0x100) == 0) continue;
        sscanf(num_str, "%d", &num);
        char* next = Line;
        int sumfreq = 0;
        for (int i=0; i<num; i++)
        {
            next = strchr(next, '\t') + 1; 
            int to, freq;
            sscanf (next, "%X: %d", &to, &freq);
            sumfreq += freq;
            sumruns -= freq * RunValue(to);
        }
        sumruns += sumfreq * RunValue(from);
    }
    printf ("%d\n", sumruns);
    fclose(fp);
}
