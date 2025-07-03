#ifndef EVRECORDDATA_H
#define EVRECORDDATA_H
#include "evRecord.h"
#include <ostream>

class evRecordData : public evRecord
{
public:
    evRecordData(std::string s);
    friend std::ostream& operator<<(std::ostream& os, const evRecordData& ev);
protected:
    std::string type;
    std::string name;
    int         value;
};

#endif /* EVRECORDDATA_H */