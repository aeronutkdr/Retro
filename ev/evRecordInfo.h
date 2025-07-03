#ifndef EVRECORDINFO_H
#define EVRECORDINFO_H
#include "evRecord.h"
#include <ostream>

class evRecordInfo : public evRecord
{
public:
    evRecordInfo(std::string s);
    friend std::ostream& operator<<(std::ostream& os, const evRecordInfo& ev);
protected:
    std::string field;
    std::string data;
};

#endif /* EVRECORDINFO_H */