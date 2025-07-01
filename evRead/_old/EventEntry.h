#ifndef EVENTENTRY_H
#define EVENTENTRY_H

struct EventLog
{
    unsigned char id;
    unsigned int  count;
};
struct EventLogNode
{
    struct EventLog data;
    struct EventLogNode* next;
};

struct EntryTransition
{
    unsigned short from;
    unsigned short to;
    unsigned int   count;
};


struct EntryTransitionNode
{
    struct EntryTransition data;
    struct EntryTransitionNode* next;
};

struct EventEntry
{
    unsigned char EntryType;
    struct EntryTransitionNode* node0;
};


struct EventEntry* EE_AddList(struct EventEntry* ee0, unsigned short *st, unsigned char num, unsigned char evt);

#endif /* EVENTENTRY_H */