#include "evPlayEvent.h"
#include <stdio.h>

int main (int argc, char* argv[])
{
    struct evParseType p;
    printf ("%s\n", evParse (argv[1], &p));
    evDump(&p);
/*
    enum evPlayEventType e;
    char* s = evParseEvent (argv[1], &e);
    printf ("%s, \"%s\": ",evString(e), s);
    char* now;
    do
    {
        now = s;
        char v;
        s = evParseNext(now, &v);
        printf ("%c,", v&(~0x80));
    } while (s!=now && *s);
    printf ("\n");
*/
    return 0;
}
