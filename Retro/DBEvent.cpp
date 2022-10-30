#include "StdAfx.h"
#include "DBEvent.h"

using namespace Retro;

System::Boolean DBEvent::TFToBoolean (System::String ^ str)
{
    System::Diagnostics::Trace::Assert (str == "\"T\"" ||
                                        str == "\"F\"");
    return str == "\"T\"";
}

System::Boolean DBEvent::RLToBoolean (System::String ^ str)
{
    System::Diagnostics::Trace::Assert (str == "\"R\"" ||
                                        str == "\"L\"");
    return str == "\"R\"";
}

System::Boolean DBEvent::ZeroOneToBoolean (System::String ^ str)
{
    System::Diagnostics::Trace::Assert (str == "0" ||
                                        str == "1");
    return str == "1";
}

DBEvent::DBEvent(void)
{
}

DBEvent::DBEvent(System::Int32   FileID,
                 System::String^ str,
                 DBDictionary^   Players,
                 DBDictionary^   Games,
                 DBDictionary^   Teams)
{
    array<System::Char>^splitChars = {','};
    array<System::Char>^trimChars = {'\"'};
    array<System::String^>^split = str->Split(splitChars);
    System::Diagnostics::Trace::Assert (split->Length == 15);
    System::Int32 GameID;
    System::Boolean NewGame = Games->Add(split[0]->Trim(trimChars), GameID);
    m_GameID = GameID;
    if (NewGame)
    {
        System::Int32 visitingTeamID = Teams->Add (split[1]->Substring(1,3));
        System::String^ Date = split[2]->Substring(4,8);
        System::DateTime dt(System::Convert::ToInt32 (Date->Substring(0,4)),
                            System::Convert::ToInt32 (Date->Substring(4,2)),
                            System::Convert::ToInt32 (Date->Substring(6,2)));
    }
            //System::Int32   m_GameID;
            //System::Int32   m_VisitingTeamID;
            //System::Byte    m_Inning;
            //System::Boolean m_HomeTeamBatting;
            //System::Byte    m_Outs;
            //System::Int32   m_BatterStart;
            //System::Int32   m_BatterRes;
            //System::Int32   m_1B;
            //System::Int32   m_2B;
            //System::Int32   m_3B;
            //System::Byte    m_OutsOnPlay;
            //System::Byte    m_DestBatter;
            //System::Byte    m_DestRunner1;
            //System::Byte    m_DestRunner2;
            //System::Byte    m_DestRunner3;

}

System::Byte DBEvent::From (System::Void)
{
    System::Byte retval = 0;
    retval |= (m_Outs << 3);
//    retval |= (!System::String::IsNullOrEmpty(m_3B)?0x04:00);
//    retval |= (!System::String::IsNullOrEmpty(m_2B)?0x02:00);
//    retval |= (!System::String::IsNullOrEmpty(m_1B)?0x01:00);
    return retval;
}

System::Byte DBEvent::To (System::Void)
{
    System::Byte retval = 0;
    retval |= ((m_Outs + m_OutsOnPlay) << 3);
    retval |= ((m_DestBatter ==3 ||
                m_DestRunner1==3 ||
                m_DestRunner2==3 ||
                m_DestRunner3==3)?0x04:0x00);
    retval |= ((m_DestBatter ==2 ||
                m_DestRunner1==2 ||
                m_DestRunner2==2 ||
                m_DestRunner3==2)?0x02:0x00);
    retval |= ((m_DestBatter ==1 ||
                m_DestRunner1==1 ||
                m_DestRunner2==1 ||
                m_DestRunner3==1)?0x01:0x00);
    return retval;
}
