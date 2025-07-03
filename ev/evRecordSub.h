#ifndef EVRECORDSUB_H
#define EVRECORDSUB_H
#include "evRecord.h"
#include <ostream>

class evRecordSub : public evRecord
{
public:
    evRecordSub(std::string s);
    friend std::ostream& operator<<(std::ostream& os, const evRecordSub& ev);
protected:
    std::string name;
    std::string fullname;
    int         home;
    int         pos;
    int         order;
};

#endif /* EVRECORDSUB_H */