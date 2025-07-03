#ifndef EVRECORDRADJ_H
#define EVRECORDRADJ_H
#include "evRecord.h"
#include <ostream>

class evRecordRadj : public evRecord
{
public:
    evRecordRadj(std::string s);
    friend std::ostream& operator<<(std::ostream& os, const evRecordRadj& ev);
    std::string getName(void) {return name;}
    int         getBase(void) {return value;}
protected:
    std::string name;
    int         value;
};

#endif /* EVRECORDRADJ_H */