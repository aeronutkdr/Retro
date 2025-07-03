#ifndef EVRECORDCOM_H
#define EVRECORDCOM_H
#include "evRecord.h"
#include <ostream>

class evRecordCom : public evRecord
{
public:
    evRecordCom(std::string s);
    friend std::ostream& operator<<(std::ostream& os, const evRecordCom& ev);
protected:
    std::string text;
};

#endif /* EVRECORDCOM_H */