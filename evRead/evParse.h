#ifndef _EVPARSE_H_
#define _EVPARSE_H_
#include "EventTypes.h"

void Parse  (char **f, char* s);
int  Process(unsigned short* to,
             char** f,
             enum EventType* et);

#endif /* _EVPARSE_H_  */