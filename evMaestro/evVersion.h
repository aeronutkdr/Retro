#ifndef _EVVERSION_H_
#define _EVVERSION_H_

struct evVersionType
{
    int val;
};

char evProcessVersion(char* str, struct evVersionType* pversion);
void evDumpVersion(struct evVersionType* pversion);

#endif /* _EVVERSION_H_ */