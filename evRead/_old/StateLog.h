#ifndef STATE_LOG_H
#define STATE_LOG_H

struct StateLogType;
struct StateToType
{
    unsigned int frequency;
    struct StateLogType* to;
    struct StateToType* next;
};
struct StateLogType
{
    unsigned int value;
    unsigned int frequency;
    struct StateToType* to0;
    struct StateLogType* next;
};

struct StateLogType* SL_Add(struct StateLogType* sl0, unsigned short* to, unsigned int n);

#endif /* STATE_LOG_H */
