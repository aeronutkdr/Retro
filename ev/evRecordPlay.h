#ifndef EVRECORDPLAY_H
#define EVRECORDPLAY_H
#include "evRecord.h"
#include <ostream>

class evRecordPlay : public evRecord
{
public:
    evRecordPlay(std::string s);
    friend std::ostream& operator<<(std::ostream& os, const evRecordPlay& ev);
    int         getInning(void)  {return inning;}
    int         getBottom(void)  {return bottom;}
    std::string getName(void)    {return name;}
    int         getBalls(void)   {return balls;}
    int         getStrikes(void) {return strikes;}
    std::string getPitches(void) {return pitches;}
    std::string getEvent(void)   {return event;}
    /* OO332211HH */      /* OO321HBBBFFFFFSS */
    /* 9876543210 */      /* FEDCBA9876543210 */
    unsigned short Process(unsigned short state);
protected:
    int         inning;
    int         bottom;
    std::string name;
    int         balls;
    int         strikes;
    std::string pitches;
    std::string event;
    unsigned short ProcessEvent(std::string ev);
};

#endif /* EVRECORDPLAY_H */