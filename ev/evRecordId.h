#ifndef EVRECORDID_H
#define EVRECORDID_H
#include "evRecord.h"
#include <ostream>

class evRecordId : public evRecord
{
public:
    evRecordId(std::string s);
    friend std::ostream& operator<<(std::ostream& os, const evRecordId& ev);
    std::string getName(void) {return name;}
protected:
    std::string name;
};

#endif /* EVRECORDID_H */