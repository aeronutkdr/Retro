#include "StdAfx.h"
#include "RetroEvent.h"

Retro::RetroEvent::RetroEvent(System::String ^evt, System::Int32 evtnum)
{
    array<System::Char>^splitChars = {','};
    array<System::Char>^trimChars = {'\"'};
    array<System::String^>^split = evt->Split(splitChars);
    System::Diagnostics::Trace::Assert (split->Length == 97);
    System::Int32 i=0;
    m_Game             = split[i]->Trim(trimChars);          i++;
    m_Team             = split[i]->Trim(trimChars);          i++;
    m_Inning           = System::Convert::ToByte (split[i]); i++;
    m_HomeTeamBatting  = ZeroOneToBoolean(split[i]);         i++;
    m_Outs             = System::Convert::ToByte (split[i]); i++;
    m_Balls            = System::Convert::ToByte (split[i]); i++;
    m_Strikes          = System::Convert::ToByte (split[i]); i++;
    m_Pitches          = split[i]->Trim(trimChars);          i++;
    m_ScoreVis         = System::Convert::ToByte (split[i]); i++;
    m_ScoreHome        = System::Convert::ToByte (split[i]); i++;
    m_BatterStart      = split[i]->Trim(trimChars);          i++;
    m_BatsRightStart   = RLToBoolean(split[i]);              i++;
    m_BatterRes        = split[i]->Trim(trimChars);          i++;
    m_BatsRightRes     = RLToBoolean(split[i]);              i++;
    m_PitcherStart     = split[i]->Trim(trimChars);          i++;
    m_PitchRightStart  = RLToBoolean(split[i]);              i++;
    m_PitcherRes       = split[i]->Trim(trimChars);          i++;
    m_PitchRightRes    = RLToBoolean(split[i]);              i++;
    m_Catcher          = split[i]->Trim(trimChars);          i++;
    m_First            = split[i]->Trim(trimChars);          i++;
    m_Second           = split[i]->Trim(trimChars);          i++;
    m_Third            = split[i]->Trim(trimChars);          i++;
    m_Shortstop        = split[i]->Trim(trimChars);          i++;
    m_Left             = split[i]->Trim(trimChars);          i++;
    m_Center           = split[i]->Trim(trimChars);          i++;
    m_Right            = split[i]->Trim(trimChars);          i++;
    m_1B               = split[i]->Trim(trimChars);          i++;
    m_2B               = split[i]->Trim(trimChars);          i++;
    m_3B               = split[i]->Trim(trimChars);          i++;
    m_EventText        = split[i]->Trim(trimChars);          i++;
    m_Leadoff          = TFToBoolean(split[i]);              i++;
    m_PinchHit         = TFToBoolean(split[i]);              i++;
    m_DefensePos       = System::Convert::ToByte (split[i]); i++;
    m_LineupPos        = System::Convert::ToByte (split[i]); i++;
    m_EventType        = System::Convert::ToByte (split[i]); i++;
    m_BatterEvent      = TFToBoolean(split[i]);              i++;
    m_AB               = TFToBoolean(split[i]);              i++;
    m_HitValue         = System::Convert::ToByte (split[i]); i++;
    m_SH               = TFToBoolean(split[i]);              i++;
    m_SF               = TFToBoolean(split[i]);              i++;
    m_OutsOnPlay       = System::Convert::ToByte (split[i]); i++;
    m_DoublePlay       = TFToBoolean(split[i]);              i++;
    m_TriplePlay       = TFToBoolean(split[i]);              i++;
    m_RBI              = System::Convert::ToByte (split[i]); i++;
    m_WildPitch        = TFToBoolean(split[i]);              i++;
    m_PassedBall       = TFToBoolean(split[i]);              i++;
    m_FieldPos         = System::Convert::ToByte (split[i]); i++;
    m_BattedBall       = split[i]->Trim(trimChars);          i++;
    m_Bunt             = TFToBoolean(split[i]);              i++;
    m_Foul             = TFToBoolean(split[i]);              i++;
    m_HitLocation      = split[i]->Trim(trimChars);          i++;
    m_NumErrors        = System::Convert::ToByte (split[i]); i++;
    m_Error1Player     = System::Convert::ToByte (split[i]); i++;
    m_Error1Type       = split[i]->Trim(trimChars);          i++;
    m_Error2Player     = System::Convert::ToByte (split[i]); i++;
    m_Error2Type       = split[i]->Trim(trimChars);          i++;
    m_Error3Player     = System::Convert::ToByte (split[i]); i++;
    m_Error3Type       = split[i]->Trim(trimChars);          i++;
    m_DestBatter       = System::Convert::ToByte (split[i]); i++;
    m_DestRunner1      = System::Convert::ToByte (split[i]); i++;
    m_DestRunner2      = System::Convert::ToByte (split[i]); i++;
    m_DestRunner3      = System::Convert::ToByte (split[i]); i++;
    m_PlayBatter       = split[i]->Trim(trimChars);          i++;
    m_PlayRunner1      = split[i]->Trim(trimChars);          i++;
    m_PlayRunner2      = split[i]->Trim(trimChars);          i++;
    m_PlayRunner3      = split[i]->Trim(trimChars);          i++;
    m_SBRunner1        = TFToBoolean(split[i]);              i++;
    m_SBRunner2        = TFToBoolean(split[i]);              i++;
    m_SBRunner3        = TFToBoolean(split[i]);              i++;
    m_CSRunner1        = TFToBoolean(split[i]);              i++;
    m_CSRunner2        = TFToBoolean(split[i]);              i++;
    m_CSRunner3        = TFToBoolean(split[i]);              i++;
    m_PORunner1        = TFToBoolean(split[i]);              i++;
    m_PORunner2        = TFToBoolean(split[i]);              i++;
    m_PORunner3        = TFToBoolean(split[i]);              i++;
    m_PitcherResp1     = split[i]->Trim(trimChars);          i++;
    m_PitcherResp2     = split[i]->Trim(trimChars);          i++;
    m_PitcherResp3     = split[i]->Trim(trimChars);          i++;
    m_GameNew          = TFToBoolean(split[i]);              i++;
    m_GameEnd          = TFToBoolean(split[i]);              i++;
    m_Pinch1           = TFToBoolean(split[i]);              i++;
    m_Pinch2           = TFToBoolean(split[i]);              i++;
    m_Pinch3           = TFToBoolean(split[i]);              i++;
    m_Runner1Removed   = split[i]->Trim(trimChars);          i++;
    m_Runner2Removed   = split[i]->Trim(trimChars);          i++;
    m_Runner3Removed   = split[i]->Trim(trimChars);          i++;
    m_BatterRemoved    = split[i]->Trim(trimChars);          i++;
    m_BatterRemovedPos = System::Convert::ToByte (split[i]); i++;
    m_FielderPO1       = System::Convert::ToByte (split[i]); i++;
    m_FielderPO2       = System::Convert::ToByte (split[i]); i++;
    m_FielderPO3       = System::Convert::ToByte (split[i]); i++;
    m_FielderAst1      = System::Convert::ToByte (split[i]); i++;
    m_FielderAst2      = System::Convert::ToByte (split[i]); i++;
    m_FielderAst3      = System::Convert::ToByte (split[i]); i++;
    m_FielderAst4      = System::Convert::ToByte (split[i]); i++;
    m_FielderAst5      = System::Convert::ToByte (split[i]); i++;
    m_EventNum         = System::Convert::ToByte (split[i]); i++;
    m_StartValue       = GetStartValue();
    m_EndValue         = GetEndValue();
    m_EvtLineNum       = evtnum;
}

System::Boolean Retro::RetroEvent::TFToBoolean (System::String ^ str)
{
    System::Diagnostics::Trace::Assert (str == "\"T\"" ||
                                        str == "\"F\"");
    return str == "\"T\"";
}

System::Boolean Retro::RetroEvent::RLToBoolean (System::String ^ str)
{
    System::Diagnostics::Trace::Assert (str == "\"R\"" ||
                                        str == "\"L\"");
    return str == "\"R\"";
}

System::Boolean Retro::RetroEvent::ZeroOneToBoolean (System::String ^ str)
{
    System::Diagnostics::Trace::Assert (str == "0" ||
                                        str == "1");
    return str == "1";
}

//System::String^ ValueStart = "[Retro].[dbo].[ValueResult] ([Retro].[dbo].[Events].[Outs],"
//                                "IIF([Retro].[dbo].[Events].[BatterID]>0,1,0),"
//                                "IIF([Retro].[dbo].[Events].[Runner1BID]>0,1,0),"
//                                "IIF([Retro].[dbo].[Events].[Runner2BID]>0,1,0),"
//                                "IIF([Retro].[dbo].[Events].[Runner3BID]>0,1,0))";
System::Byte Retro::RetroEvent::GetStartValue (System::Void)
{
    return GetValue (m_Outs,
                     !System::String::IsNullOrEmpty(m_BatterStart),
                     !System::String::IsNullOrEmpty(m_1B),
                     !System::String::IsNullOrEmpty(m_2B),
                     !System::String::IsNullOrEmpty(m_3B));
}

//System::String^ ValueEnd = "[Retro].[dbo].[ValueResult] ([Retro].[dbo].[Events].[Outs]+[OutsOnPlay],"
//                            " 1-[Retro].[dbo].[Events].[BatterEvent],"
//                            " IIF([Retro].[dbo].[Events].[DestBatter]=1 OR"
//                                " [Retro].[dbo].[Events].[DestRunner1]=1,1,0),"
//                            " IIF([Retro].[dbo].[Events].[DestBatter]=2 OR"
//                                " [Retro].[dbo].[Events].[DestRunner1]=2 OR"
//                                " [Retro].[dbo].[Events].[DestRunner2]=2,1,0),"
//                            " IIF([Retro].[dbo].[Events].[DestBatter]=3 OR"
//                                " [Retro].[dbo].[Events].[DestRunner1]=3 OR"
//                                " [Retro].[dbo].[Events].[DestRunner2]=3 OR"
//                                " [Retro].[dbo].[Events].[DestRunner3]=3,1,0))";
System::Byte Retro::RetroEvent::GetEndValue   (System::Void)
{
    return GetValue (m_Outs + m_OutsOnPlay,
                     !m_BatterEvent,
                     ((m_DestBatter == 1) ||
                      (m_DestRunner1 == 1)),
                     ((m_DestBatter == 2) ||
                      (m_DestRunner1 == 2) ||
                      (m_DestRunner2 == 2)),
                     ((m_DestBatter == 3) ||
                      (m_DestRunner1 == 3) ||
                      (m_DestRunner2 == 3) ||
                      (m_DestRunner3 == 3)));
}

System::Byte Retro::RetroEvent::GetValue (System::Byte out,
                                          System::Boolean B0,
                                          System::Boolean B1,
                                          System::Boolean B2,
                                          System::Boolean B3)
{
    return (out << 4) +
           (((System::Byte)B3) << 3) +
           (((System::Byte)B2) << 2) +
           (((System::Byte)B1) << 1) +
           (((System::Byte)B0) << 0);
}