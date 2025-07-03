#include "evRecordVersion.h"
#include <cassert>
#include <iostream>

evRecordVersion::evRecordVersion(std::string s)
{
    num = std::stoi(s);
}

std::ostream& operator<<(std::ostream& os, const evRecordVersion& ev)
{
    os << "Version/"
       << ev.num;
    return os;
}