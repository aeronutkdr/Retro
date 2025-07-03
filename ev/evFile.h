#ifndef EVFILE_H
#define EVFILE_H
#include <fstream>
#include <string>
#include "evRecord.h"
#include "evRecordId.h"
#include "evRecordPlay.h"
#include "evRecordCom.h"
#include "evRecordData.h"
#include "evRecordInfo.h"
#include "evRecordRadj.h"
#include "evRecordStart.h"
#include "evRecordSub.h"
#include "evRecordVersion.h"

class evFile
{
    public:
        evFile(std::string str);
        ~evFile(void);
        enum evRecordType Next(void);
        evRecordId      FetchId     (void) {return evRecordId     (line);}
        evRecordPlay    FetchPlay   (void) {return evRecordPlay   (line);}
        evRecordCom     FetchCom    (void) {return evRecordCom    (line);}
        evRecordData    FetchData   (void) {return evRecordData   (line);}
        evRecordInfo    FetchInfo   (void) {return evRecordInfo   (line);}
        evRecordRadj    FetchRadj   (void) {return evRecordRadj   (line);}
        evRecordStart   FetchStart  (void) {return evRecordStart  (line);}
        evRecordSub     FetchSub    (void) {return evRecordSub    (line);}
        evRecordVersion FetchVersion(void) {return evRecordVersion(line);}
    protected:
        std::ifstream f;
        std::string   line;
};
#endif /* EVFILE_H */
