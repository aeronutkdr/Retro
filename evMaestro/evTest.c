#include "ev.h"
#include <stdio.h>
#include <string.h>
#include <dirent.h>

#define LINE_LEN (2047)
int main (int argc, char* argv[])
{
    DIR* d = opendir("..\\1999eve");
    struct dirent* e;
    char Line[LINE_LEN + 1];
    FILE* fp;

    while (e = readdir(d))
    {
        if ((strstr(e->d_name, ".ROS")) &&
            strncmp(e->d_name, "ALS", 3))
        {
            strcpy(Line, "..\\1999eve\\");
            strcat(Line, e->d_name);
            fp = fopen(Line, "r");
            while (fgets(Line, LINE_LEN, fp) != NULL)
            {
                *strchr(Line,'\n') = 0;
                evRosterAdd(Line);
            }
            fclose(fp);
        }
    }
    (void) closedir(d);
    //evRosterDump();
    struct ev myEv;
    fp = fopen(argv[1], "r");
    int init = 0;
    while (fgets(Line, LINE_LEN, fp) != NULL)
    {
        *strchr(Line, '\n') = 0;
        evCreate (Line, &myEv);
        if (init && myEv.type == evID);
        init = 1;
        evProcess (&myEv);
        //evDump(&myEv);
    }
    fclose(fp);
    return 0;
}
