#ifndef EVRECORDSTART_H
#define EVRECORDSTART_H
#include "evRecord.h"
#include <ostream>

class evRecordStart : public evRecord
{
public:
    evRecordStart(std::string s);
    friend std::ostream& operator<<(std::ostream& os, const evRecordStart& ev);
    int         getHome(void)  {return home;}
    int         getOrder(void) {return order;}
    int         getPos(void)   {return pos;}
    std::string getName(void)  {return name;}
protected:
    std::string name;
    std::string fullname;
    int         home;
    int         order;
    int         pos;
};

#endif /* EVRECORDSTART_H */