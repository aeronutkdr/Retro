#include "evGame.h"
#include <stdio.h>

#define LINE_LEN (512)
int main (int argc, char* argv[])
{
    char Line[LINE_LEN + 1];
    int i=0;
    struct evGame g;
    FILE* fp = fopen (argv[1], "r");
    evGameInit(&g);
    while (fgets(Line, LINE_LEN, fp)[0] != NULL)
    {
        i++;
        if (evGameProcessLine(Line))
        {
            // game is complete
        }
    }
    (void) fclose(fp);
    return 0;
}
