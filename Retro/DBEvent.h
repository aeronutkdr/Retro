#pragma once
#include "DBDictionary.h"

namespace Retro
{
    public ref class DBEvent
    {
        protected:
            System::String^ m_GameName;
            System::String^ m_VisitTeam;
            //System::Byte    m_Inning;
            //System::Boolean m_HomeTeamBatting;
            //System::Byte    m_Outs;
            //System::String^ m_BatterStart;
            //System::String^ m_BatterRes;

            System::Int32   m_GameID;
            //System::Int32   m_VisitingTeamID;
            System::Byte    m_Inning;
            System::Boolean m_HomeTeamBatting;
            System::Byte    m_Outs;
            System::Int32   m_BatterStart;
            System::Int32   m_BatterRes;
            System::Int32   m_1B;
            System::Int32   m_2B;
            System::Int32   m_3B;
            System::Byte    m_OutsOnPlay;
            System::Byte    m_DestBatter;
            System::Byte    m_DestRunner1;
            System::Byte    m_DestRunner2;
            System::Byte    m_DestRunner3;

            System::Boolean TFToBoolean      (System::String ^ str);
            System::Boolean RLToBoolean      (System::String ^ str);
            System::Boolean ZeroOneToBoolean (System::String ^ str);
        public:
            DBEvent(System::Int32   FileID,
                    System::String^ str,
                    DBDictionary^   Players,
                    DBDictionary^   Games,
                    DBDictionary^   Teams);
            System::Byte From (System::Void);
            System::Byte To   (System::Void);
        public:
            DBEvent(void);
    };
}