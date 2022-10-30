#include "StdAfx.h"
#include "Event.h"

using namespace Retro;

System::Boolean Event::TFToBoolean (System::String ^ str)
{
    System::Diagnostics::Trace::Assert (str == "\"T\"" ||
                                        str == "\"F\"");
    return str == "\"T\"";
}

System::Boolean Event::RLToBoolean (System::String ^ str)
{
    System::Diagnostics::Trace::Assert (str == "\"R\"" ||
                                        str == "\"L\"");
    return str == "\"R\"";
}

System::Boolean Event::ZeroOneToBoolean (System::String ^ str)
{
    System::Diagnostics::Trace::Assert (str == "0" ||
                                        str == "1");
    return str == "1";
}

Event::Event(System::String^ str,
             StringList^     Players,
             StringList^     Games,
             StringList^     Teams)
{
    array<System::Char>^splitChars = {','};
    array<System::Char>^trimChars = {'\"'};
    array<System::String^>^split = str->Split(splitChars);
    System::Diagnostics::Trace::Assert (split->Length == 97);

    m_Game             = Games->Add(split[ 0]->Trim(trimChars));
    m_Team             = Teams->Add(split[ 1]->Trim(trimChars));
    m_Inning           = System::Convert::ToByte (split[ 2]);
    m_HomeTeamBatting  = ZeroOneToBoolean(split[ 3]);
    m_Outs             = System::Convert::ToByte (split[ 4]);
    m_Balls            = System::Convert::ToByte (split[ 5]);
    m_Strikes          = System::Convert::ToByte (split[ 6]);
    m_Pitches          = split[ 7]->Trim(trimChars);
    m_ScoreVis         = System::Convert::ToByte (split[ 8]);
    m_ScoreHome        = System::Convert::ToByte (split[ 9]);
    m_BatterStart      = Players->Add (split[10]->Trim(trimChars));
    m_BatsRightStart   = RLToBoolean(split[11]);
    m_BatterRes        = Players->Add (split[12]->Trim(trimChars));
    m_BatsRightRes     = RLToBoolean(split[13]);
    m_PitcherStart     = Players->Add (split[14]->Trim(trimChars));
    m_PitchRightStart  = RLToBoolean(split[15]);
    m_PitcherRes       = Players->Add (split[16]->Trim(trimChars));
    m_PitchRightRes    = RLToBoolean(split[17]);
    m_Catcher          = Players->Add (split[18]->Trim(trimChars));
    m_First            = Players->Add (split[19]->Trim(trimChars));
    m_Second           = Players->Add (split[20]->Trim(trimChars));
    m_Third            = Players->Add (split[21]->Trim(trimChars));
    m_Shortstop        = Players->Add (split[22]->Trim(trimChars));
    m_Left             = Players->Add (split[23]->Trim(trimChars));
    m_Center           = Players->Add (split[24]->Trim(trimChars));
    m_Right            = Players->Add (split[25]->Trim(trimChars));
    m_1B               = Players->Add (split[26]->Trim(trimChars));
    m_2B               = Players->Add (split[27]->Trim(trimChars));
    m_3B               = Players->Add (split[28]->Trim(trimChars));
    m_EventText        = split[29]->Trim(trimChars);
    m_Leadoff          = TFToBoolean(split[30]);
    m_PinchHit         = TFToBoolean(split[31]);
    m_DefensePos       = System::Convert::ToByte (split[32]);
    m_LineupPos        = System::Convert::ToByte (split[33]);
    m_EventType        = System::Convert::ToByte (split[34]);
    m_BatterEvent      = TFToBoolean(split[35]);
    m_AB               = TFToBoolean(split[36]);
    m_HitValue         = System::Convert::ToByte (split[37]);
    m_SH               = TFToBoolean(split[38]);
    m_SF               = TFToBoolean(split[39]);
    m_OutsOnPlay       = System::Convert::ToByte (split[40]);
    m_DoublePlay       = TFToBoolean(split[41]);
    m_TriplePlay       = TFToBoolean(split[42]);
    m_RBI              = System::Convert::ToByte (split[43]);
    m_WildPitch        = TFToBoolean(split[44]);
    m_PassedBall       = TFToBoolean(split[45]);
    m_FieldPos         = System::Convert::ToByte (split[46]);
    m_BattedBall       = split[47]->Trim(trimChars);
    m_Bunt             = TFToBoolean(split[48]);
    m_Foul             = TFToBoolean(split[49]);
    m_HitLocation      = split[50]->Trim(trimChars);
    m_NumErrors        = System::Convert::ToByte (split[51]);
    m_Error1Player     = System::Convert::ToByte (split[52]);
    m_Error1Type       = split[53]->Trim(trimChars);
    m_Error2Player     = System::Convert::ToByte (split[54]);
    m_Error2Type       = split[55]->Trim(trimChars);
    m_Error3Player     = System::Convert::ToByte (split[56]);
    m_Error3Type       = split[57]->Trim(trimChars);
    m_DestBatter       = System::Convert::ToByte (split[58]);
    m_DestRunner1      = System::Convert::ToByte (split[59]);
    m_DestRunner2      = System::Convert::ToByte (split[60]);
    m_DestRunner3      = System::Convert::ToByte (split[61]);
    m_PlayBatter       = split[62]->Trim(trimChars);
    m_PlayRunner1      = split[63]->Trim(trimChars);
    m_PlayRunner2      = split[64]->Trim(trimChars);
    m_PlayRunner3      = split[65]->Trim(trimChars);
    m_SBRunner1        = TFToBoolean(split[66]);
    m_SBRunner2        = TFToBoolean(split[67]);
    m_SBRunner3        = TFToBoolean(split[68]);
    m_CSRunner1        = TFToBoolean(split[69]);
    m_CSRunner2        = TFToBoolean(split[70]);
    m_CSRunner3        = TFToBoolean(split[71]);
    m_PORunner1        = TFToBoolean(split[72]);
    m_PORunner2        = TFToBoolean(split[73]);
    m_PORunner3        = TFToBoolean(split[74]);
    m_PitcherResp1     = Players->Add (split[75]->Trim(trimChars));
    m_PitcherResp2     = Players->Add (split[76]->Trim(trimChars));
    m_PitcherResp3     = Players->Add (split[77]->Trim(trimChars));
    m_GameNew          = TFToBoolean(split[78]);
    m_GameEnd          = TFToBoolean(split[79]);
    m_Pinch1           = TFToBoolean(split[80]);
    m_Pinch2           = TFToBoolean(split[81]);
    m_Pinch3           = TFToBoolean(split[82]);
    m_Runner1Removed   = Players->Add (split[83]->Trim(trimChars));
    m_Runner2Removed   = Players->Add (split[84]->Trim(trimChars));
    m_Runner3Removed   = Players->Add (split[85]->Trim(trimChars));
    m_BatterRemoved    = Players->Add (split[86]->Trim(trimChars));
    m_BatterRemovedPos = System::Convert::ToByte (split[87]);
    m_FielderPO1       = System::Convert::ToByte (split[88]);
    m_FielderPO2       = System::Convert::ToByte (split[89]);
    m_FielderPO3       = System::Convert::ToByte (split[90]);
    m_FielderAst1      = System::Convert::ToByte (split[91]);
    m_FielderAst2      = System::Convert::ToByte (split[92]);
    m_FielderAst3      = System::Convert::ToByte (split[93]);
    m_FielderAst4      = System::Convert::ToByte (split[94]);
    m_FielderAst5      = System::Convert::ToByte (split[95]);
    m_EventNum         = System::Convert::ToByte (split[96]);
}

System::Byte Event::From (System::Void)
{
    System::Byte retval = 0;
    retval |= (m_Outs << 3);
    retval |= ((m_3B != -1)?0x04:00);
    retval |= ((m_2B != -1)?0x02:00);
    retval |= ((m_1B != -1)?0x01:00);
    return retval;
}

System::Byte Event::To   (System::Void)
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
