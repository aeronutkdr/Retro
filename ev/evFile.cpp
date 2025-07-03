#include "evFile.h"
#include <iostream>
#include <cassert>
#include <string>
#include "evRecordPlay.h"
#include "evRecordId.h"

evFile::evFile(std::string str)
{
    line = str;
    //std::cout << line << std::endl;
    f.open (str, std::ios_base::in);
    assert(f);
}

evFile::~evFile(void)
{
    //std::cout << "~" << line << std::endl;
    f.close();
}

evRecordType evFile::Next(void)
{
    evRecordType retval = evrtNone;
    if (!f.eof())
    {
        std::getline(f, line);
             if (line.find("com,"    ) == 0) {retval = evrtCom;     line = line.substr(4);}
        else if (line.find("data,"   ) == 0) {retval = evrtData;    line = line.substr(5);}
        else if (line.find("id,"     ) == 0) {retval = evrtId;      line = line.substr(3);}
        else if (line.find("info,"   ) == 0) {retval = evrtInfo;    line = line.substr(5);}
        else if (line.find("play,"   ) == 0) {retval = evrtPlay;    line = line.substr(5);}
        else if (line.find("radj,"   ) == 0) {retval = evrtRadj;    line = line.substr(5);}
        else if (line.find("start,"  ) == 0) {retval = evrtStart;   line = line.substr(6);}
        else if (line.find("sub,"    ) == 0) {retval = evrtSub;     line = line.substr(4);}
        else if (line.find("version,") == 0) {retval = evrtVersion; line = line.substr(8);}
    }
    return retval;
}