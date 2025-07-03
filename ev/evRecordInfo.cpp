#include "evRecordInfo.h"
#include <cassert>
#include <iostream>

evRecordInfo::evRecordInfo(std::string s)
{
    std::size_t r = 0;
    std::size_t t = s.find_first_of(',');
    int i;
    for (i=0; t != s.npos; i++, r = t+1, t = s.find_first_of(',', r))
    {
        switch (i)
        {
            case 0:
                assert ((t-r) > 0);
                field    = s.substr(r,t-r); break;
            default: break;
        }
    }
    assert (i==1);
    data = s.substr(r);
}

std::ostream& operator<<(std::ostream& os, const evRecordInfo& ev)
{
    os << "Info/"
       << ev.field  << '/'
       << ev.data;
    return os;
}