#include "StdAfx.h"
#include "FileEvent.h"

using namespace Retro;

FileEvent::FileEvent(void)
{
}

System::Boolean FileEvent::ZeroOneToBoolean (System::String ^ str)
{
    System::Diagnostics::Trace::Assert (str == "0" ||
                                        str == "1");
    return str == "1";
}

FileEvent::FileEvent(System::String^ e)
{
    array<System::String^>^split = e->Split(splitChars);
    System::Diagnostics::Trace::Assert (split->Length == 14);
    m_GameName        = split[0]->Trim(trimChars);
    m_VisitTeam       = split[1]->Trim(trimChars);
    m_Inning          = System::Convert::ToByte (split[2]);
    m_HomeTeamBatting = ZeroOneToBoolean(split[3]);
    m_Outs            = System::Convert::ToByte (split[4]);
    m_BatterStart     = split[5]->Trim(trimChars);
    m_Runner1         = split[6]->Trim(trimChars);
    m_Runner2         = split[7]->Trim(trimChars);
    m_Runner3         = split[8]->Trim(trimChars);
    m_OutsOnPlay      = System::Convert::ToByte (split[9]);
    m_BatterDest      = System::Convert::ToByte (split[10]);
    m_Runner1Dest     = System::Convert::ToByte (split[11]);
    m_Runner2Dest     = System::Convert::ToByte (split[12]);
    m_Runner3Dest     = System::Convert::ToByte (split[13]);
}

System::String^ FileEvent::ToString(System::Void)
{
    System::String^ retval = "";
    retval += "m_GameName        = " + m_GameName        + System::Environment::NewLine;
    retval += "m_VisitTeam       = " + m_VisitTeam       + System::Environment::NewLine;
    retval += m_HomeTeamBatting?"Bot ":"Top " + m_Inning + System::Environment::NewLine;
    retval += "m_Outs            = " + m_Outs            + System::Environment::NewLine;
    retval += "m_BatterStart     = " + m_BatterStart     + System::Environment::NewLine;
    retval += "m_Runner1         = " + m_Runner1         + System::Environment::NewLine;
    retval += "m_Runner2         = " + m_Runner2         + System::Environment::NewLine;
    retval += "m_Runner3         = " + m_Runner3         + System::Environment::NewLine;
    retval += "m_OutsOnPlay      = " + m_OutsOnPlay      + System::Environment::NewLine;
    retval += "m_BatterDest      = " + m_BatterDest      + System::Environment::NewLine;
    retval += "m_Runner1Dest     = " + m_Runner1Dest     + System::Environment::NewLine;
    retval += "m_Runner2Dest     = " + m_Runner2Dest     + System::Environment::NewLine;
    retval += "m_Runner3Dest     = " + m_Runner3Dest     + System::Environment::NewLine;
    return retval;
}

System::Byte FileEvent::From (System::Void)
{
    System::Byte retval = 0;
    retval |= (m_Outs << 3);
    retval |= (System::String::IsNullOrEmpty(m_Runner1)?0x00:0x04);
    retval |= (System::String::IsNullOrEmpty(m_Runner2)?0x00:0x02);
    retval |= (System::String::IsNullOrEmpty(m_Runner3)?0x00:0x01);
    return retval;
}

System::Byte FileEvent::To   (System::Void)
{
    System::Byte retval = 0;
    retval |= ((m_Outs + m_OutsOnPlay) << 3);
    retval |= ((m_BatterDest ==3 ||
                m_Runner1Dest==3 ||
                m_Runner2Dest==3 ||
                m_Runner3Dest==3)?0x04:0x00);
    retval |= ((m_BatterDest ==2 ||
                m_Runner1Dest==2 ||
                m_Runner2Dest==2 ||
                m_Runner3Dest==2)?0x02:0x00);
    retval |= ((m_BatterDest ==1 ||
                m_Runner1Dest==1 ||
                m_Runner2Dest==1 ||
                m_Runner3Dest==1)?0x01:0x00);
    return retval;
}
