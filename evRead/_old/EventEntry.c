#include "EventEntry.h"

struct EventEntry* EE_AddList(struct EventEntry* ee0, unsigned short *st, unsigned char num, unsigned char evt)
{
    unsigned int trans = st[0];
    for (int i=1; i<num; i++)
    {
        trans <<= 16;
        trans |= st[i];
        struct EventEntry* evt = EE_Find(ee0, trans);
        if (evt == 0)
        {
            evt = malloc (sizeof (struct EventEntry));
            evt->EntryType = -1;
            evt->next = ee0;
            ee0 = evt;
        }
    }

}