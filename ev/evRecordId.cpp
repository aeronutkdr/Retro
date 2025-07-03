#include "evRecordId.h"
#include <iostream>

evRecordId::evRecordId(std::string s)
{
    name = s;
}

std::ostream& operator<<(std::ostream& os, const evRecordId& ev)
{
    os << "Id/"
       << ev.name;
    return os;
}