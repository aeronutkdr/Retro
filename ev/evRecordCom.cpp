#include "evRecordCom.h"
#include <cassert>
#include <iostream>

evRecordCom::evRecordCom(std::string s)
{
    text = s.substr(1,s.length()-2);
}

std::ostream& operator<<(std::ostream& os, const evRecordCom& ev)
{
    os << "Com/"
       << ev.text;
    return os;
}