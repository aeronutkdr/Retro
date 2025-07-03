#include "evRecordData.h"
#include <cassert>
#include <iostream>

evRecordData::evRecordData(std::string s)
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
                type = s.substr(r,t-r); break;
            case 1:
                assert ((t-r) == 8);
                name = s.substr(r,t-r); break;
            default: break;
        }
    }
    assert (i==2);
    value  = std::stoi(s.substr(r));
}

std::ostream& operator<<(std::ostream& os, const evRecordData& ev)
{
    os << "Data/"
       << ev.type  << '/'
       << ev.name  << '/'
       << ev.value;
    return os;
}