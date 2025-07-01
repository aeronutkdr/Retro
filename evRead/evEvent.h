#ifndef _EVEVENT_H_
#define _EVEVENT_H_
#include "eventTypes.h"

struct evEventTransition
{
    struct evEventTransition* next;
    unsigned int              freq;
    unsigned short            transition; /* ((From>>10)<<8) | (To>>10))
                                          /* ((From>>2) & 0x3F00) | ((to>>10) & 0x3F) */
};

struct evEvent
{
    struct evEventTransition* evTrans0;
    struct evEvent*           next;
    enum EventType            event;
};

struct evEvent* evEvent_Add(struct evEvent* root,
                            enum EventType e,
                            unsigned short f,
                            unsigned short t);
void evEvent_Dump(struct evEvent* root);
void evEvent_Free(struct evEvent* root);

#endif /* _EVEVENT_H_ */
