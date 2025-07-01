#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "PitchSequence.h"
int main(int argc, char* argv[])
{
    int len = strlen(argv[1]);
    int* Counts = malloc(len * sizeof(int));
    len = PitchSequence(argv[1], Counts, len);
    for (int i=0; i<len; i++)
    {
         printf("%04X\n", Counts[i]);
    }
    free(Counts);
    return 0;
}