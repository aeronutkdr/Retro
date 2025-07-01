#include <memory.h>
#include <stdio.h>
#include "evInfo.h"
#include "evPlay.h"
#include "evData.h"
#include "evGameID.h"
#include "evStartSub.h"
#include "evVersion.h"
#include "evBadj.h"
#include "evPrimitives.h"
#include "evRoster.h"
#include "evGame.h"

int main (int argc, char* argv[])
{
    char Line[201];
    int i=0;
#if 1
    //struct evGame g;
    while (fgets(Line, 200, stdin)[0] != '\n')
    {
        if (!memcmp(Line, PLAY_STR, 4))
        {
            i++;
            struct evPlayType aPlay;
            evProcessPlay(Line+5, &aPlay);
            printf ("%d ", i);
            //evSetPlay(&g, &aPlay);
            //DumpPlay(&aPlay);
        }
    }
#elif 1
    while (fgets(Line, 200, stdin)[0] != '\n')
    {
        evRoster_Add(Line);
    }
    evRoster_Dump (evRoster_Get("lukem001"));
    printf ("****\n");
    evRoster_DumpList();
    printf ("****\n");
    evRoster_Free();
#else
    struct InfoType aInfo;
    while (fgets(Line, 200, stdin)[0] != '\n')
    {
        if (memchr(Line, '\n', 200)) * (char*) memchr(Line, '\n', 200) = 0;
        if (!memcmp(Line, INFO_STR   , 4)) {                              ProcessInfo    (Line+5, &aInfo);                            }
        if (!memcmp(Line, PLAY_STR   , 4)) {struct evPlayType     aPlay;    ProcessPlay    (Line+5, &aPlay);    DumpPlay    (&aPlay);   }
        if (!memcmp(Line, ID_STR     , 2)) {struct IDType       aID;      ProcessID      (Line+3, &aID);      DumpID      (&aID);     }
        if (!memcmp(Line, VERSION_STR, 7)) {struct VersionType  aVersion; ProcessVersion (Line+8, &aVersion); DumpVersion (&aVersion);}
        if (!memcmp(Line, START_STR  , 5)) {struct StartSubType aStart;   ProcessStartSub(Line+6, &aStart);   DumpStartSub(&aStart);  }
        if (!memcmp(Line, SUB_STR    , 3)) {struct StartSubType aSub;     ProcessStartSub(Line+4, &aSub);     DumpStartSub(&aSub);    }
        if (!memcmp(Line, DATA_STR   , 4)) {struct DataType     aData;    ProcessData    (Line+5, &aData);    DumpData    (&aData);   }
        if (!memcmp(Line, COM_STR    , 3));
        if (!memcmp(Line, BADJ_STR   , 4)) {struct BadjType     aBadj;    ProcessBadj    (Line+5, &aBadj);    DumpBadj    (&aBadj);   }
    }
    //DumpInfo (&aInfo);
#endif
    return 0;
}