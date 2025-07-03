#ifndef EVRECORDVERSION_H
#define EVRECORDVERSION_H
#include "evRecord.h"
#include <ostream>

class evRecordVersion : public evRecord
{
public:
    evRecordVersion(std::string s);
    friend std::ostream& operator<<(std::ostream& os, const evRecordVersion& ev);
    int getVersion(void) {return num;}
protected:
    int num;
};

#endif /* EVRECORDVERSION_H */