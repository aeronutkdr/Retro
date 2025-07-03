#include "evRecordStart.h"
#include <cassert>
#include <iostream>

evRecordStart::evRecordStart(std::string s)
{
    //std::cout << s << std::endl;
    std::size_t r = 0;
    std::size_t t = s.find_first_of(',');
    int i;
    for (i=0; t != s.npos; i++, r = t+1, t = s.find_first_of(',', r))
    {
        switch (i)
        {
            case 0:
                assert ((t-r) == 8);
                name     = s.substr(r,t-r);            break;
            case 1:
                assert ((t-r) > 2);
                fullname = s.substr(r+1,t-r-2);        break;
            case 2:
                assert ((t-r) > 0);
                home     = std::stoi(s.substr(r,t-r)); break;
            case 3:
                assert ((t-r) == 1);
                order    = std::stoi(s.substr(r,t-r)); break;
            default: break;
        }
    }
    assert (i==4);
    pos = std::stoi(s.substr(r));
}

std::ostream& operator<<(std::ostream& os, const evRecordStart& ev)
{
    os << "Start/"
       << ev.name     << '/'
       << ev.fullname << '/'
       << ev.home     << '/'
       << ev.pos      << '/'
       << ev.order;
    return os;
}