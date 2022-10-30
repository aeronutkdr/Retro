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
             StringList^     Teams,
             System::Boolean Full)
{
    array<System::Char>^splitChars = {','};
    array<System::Char>^trimChars = {'\"'};
    array<System::String^>^split = str->Split(splitChars);
    System::Diagnostics::Trace::Assert (( Full && split->Length == 97) ||
                                        (!Full && split->Length == 15));
    System::Int32 i=0;
    /* -f 0-4,10,26-28,35,40,58-61 */
    /* -f 0-97 */
               m_GameName         = split[i]->Trim(trimChars);    i++;
               m_Game             = Games->Add(m_GameName);
               m_Team             = Teams->Add(split[i]->Trim(trimChars));    i++;
               m_Inning           = System::Convert::ToByte (split[i]);       i++;
               m_HomeTeamBatting  = ZeroOneToBoolean(split[i]);               i++;
               m_Outs             = System::Convert::ToByte (split[i]);       i++;
    if (Full) {m_Balls            = System::Convert::ToByte (split[i]);       i++;
               m_Strikes          = System::Convert::ToByte (split[i]);       i++;
               m_Pitches          = split[i]->Trim(trimChars);                i++;
               m_ScoreVis         = System::Convert::ToByte (split[i]);       i++;
               m_ScoreHome        = System::Convert::ToByte (split[i]);       i++;}
               m_BatterStart      = Players->Add (split[i]->Trim(trimChars)); i++;
    if (Full) {m_BatsRightStart   = RLToBoolean(split[i]);                    i++;
               m_BatterRes        = Players->Add (split[i]->Trim(trimChars)); i++;
               m_BatsRightRes     = RLToBoolean(split[i]);                    i++;
               m_PitcherStart     = Players->Add (split[i]->Trim(trimChars)); i++;
               m_PitchRightStart  = RLToBoolean(split[i]);                    i++;
               m_PitcherRes       = Players->Add (split[i]->Trim(trimChars)); i++;
               m_PitchRightRes    = RLToBoolean(split[i]);                    i++;
               m_Catcher          = Players->Add (split[i]->Trim(trimChars)); i++;
               m_First            = Players->Add (split[i]->Trim(trimChars)); i++;
               m_Second           = Players->Add (split[i]->Trim(trimChars)); i++;
               m_Third            = Players->Add (split[i]->Trim(trimChars)); i++;
               m_Shortstop        = Players->Add (split[i]->Trim(trimChars)); i++;
               m_Left             = Players->Add (split[i]->Trim(trimChars)); i++;
               m_Center           = Players->Add (split[i]->Trim(trimChars)); i++;
               m_Right            = Players->Add (split[i]->Trim(trimChars)); i++;}
               m_1B               = Players->Add (split[i]->Trim(trimChars)); i++;
               m_2B               = Players->Add (split[i]->Trim(trimChars)); i++;
               m_3B               = Players->Add (split[i]->Trim(trimChars)); i++;
    if (Full) {m_EventText        = split[i]->Trim(trimChars);                i++;
               m_Leadoff          = TFToBoolean(split[i]);                    i++;
               m_PinchHit         = TFToBoolean(split[i]);                    i++;
               m_DefensePos       = System::Convert::ToByte (split[i]);       i++;
               m_LineupPos        = System::Convert::ToByte (split[i]);       i++;
               m_EventType        = System::Convert::ToByte (split[i]);       i++;}
               m_BatterEvent      = TFToBoolean(split[i]);                    i++;
    if (Full) {m_AB               = TFToBoolean(split[i]);                    i++;
               m_HitValue         = System::Convert::ToByte (split[i]);       i++;
               m_SH               = TFToBoolean(split[i]);                    i++;
               m_SF               = TFToBoolean(split[i]);                    i++;}
               m_OutsOnPlay       = System::Convert::ToByte (split[i]);       i++;
    if (Full) {m_DoublePlay       = TFToBoolean(split[i]);                    i++;
               m_TriplePlay       = TFToBoolean(split[i]);                    i++;
               m_RBI              = System::Convert::ToByte (split[i]);       i++;
               m_WildPitch        = TFToBoolean(split[i]);                    i++;
               m_PassedBall       = TFToBoolean(split[i]);                    i++;
               m_FieldPos         = System::Convert::ToByte (split[i]);       i++;
               m_BattedBall       = split[i]->Trim(trimChars);                i++;
               m_Bunt             = TFToBoolean(split[i]);                    i++;
               m_Foul             = TFToBoolean(split[i]);                    i++;
               m_HitLocation      = split[i]->Trim(trimChars);                i++;
               m_NumErrors        = System::Convert::ToByte (split[i]);       i++;
               m_Error1Player     = System::Convert::ToByte (split[i]);       i++;
               m_Error1Type       = split[i]->Trim(trimChars);                i++;
               m_Error2Player     = System::Convert::ToByte (split[i]);       i++;
               m_Error2Type       = split[i]->Trim(trimChars);                i++;
               m_Error3Player     = System::Convert::ToByte (split[i]);       i++;
               m_Error3Type       = split[i]->Trim(trimChars);                i++;}
               m_DestBatter       = System::Convert::ToSByte (split[i]);      i++;
               m_DestRunner1      = System::Convert::ToSByte (split[i]);      i++;
               m_DestRunner2      = System::Convert::ToSByte (split[i]);      i++;
               m_DestRunner3      = System::Convert::ToSByte (split[i]);      i++;
    if (Full) {m_PlayBatter       = split[i]->Trim(trimChars);                i++;
               m_PlayRunner1      = split[i]->Trim(trimChars);                i++;
               m_PlayRunner2      = split[i]->Trim(trimChars);                i++;
               m_PlayRunner3      = split[i]->Trim(trimChars);                i++;
               m_SBRunner1        = TFToBoolean(split[i]);                    i++;
               m_SBRunner2        = TFToBoolean(split[i]);                    i++;
               m_SBRunner3        = TFToBoolean(split[i]);                    i++;
               m_CSRunner1        = TFToBoolean(split[i]);                    i++;
               m_CSRunner2        = TFToBoolean(split[i]);                    i++;
               m_CSRunner3        = TFToBoolean(split[i]);                    i++;
               m_PORunner1        = TFToBoolean(split[i]);                    i++;
               m_PORunner2        = TFToBoolean(split[i]);                    i++;
               m_PORunner3        = TFToBoolean(split[i]);                    i++;
               m_PitcherResp1     = Players->Add (split[i]->Trim(trimChars)); i++;
               m_PitcherResp2     = Players->Add (split[i]->Trim(trimChars)); i++;
               m_PitcherResp3     = Players->Add (split[i]->Trim(trimChars)); i++;
               m_GameNew          = TFToBoolean(split[i]);                    i++;
               m_GameEnd          = TFToBoolean(split[i]);                    i++;
               m_Pinch1           = TFToBoolean(split[i]);                    i++;
               m_Pinch2           = TFToBoolean(split[i]);                    i++;
               m_Pinch3           = TFToBoolean(split[i]);                    i++;
               m_Runner1Removed   = Players->Add (split[i]->Trim(trimChars)); i++;
               m_Runner2Removed   = Players->Add (split[i]->Trim(trimChars)); i++;
               m_Runner3Removed   = Players->Add (split[i]->Trim(trimChars)); i++;
               m_BatterRemoved    = Players->Add (split[i]->Trim(trimChars)); i++;
               m_BatterRemovedPos = System::Convert::ToByte (split[i]);       i++;
               m_FielderPO1       = System::Convert::ToByte (split[i]);       i++;
               m_FielderPO2       = System::Convert::ToByte (split[i]);       i++;
               m_FielderPO3       = System::Convert::ToByte (split[i]);       i++;
               m_FielderAst1      = System::Convert::ToByte (split[i]);       i++;
               m_FielderAst2      = System::Convert::ToByte (split[i]);       i++;
               m_FielderAst3      = System::Convert::ToByte (split[i]);       i++;
               m_FielderAst4      = System::Convert::ToByte (split[i]);       i++;
               m_FielderAst5      = System::Convert::ToByte (split[i]);       i++;
               m_EventNum         = System::Convert::ToByte (split[i]);       i++;}
    //post process...
    System::Diagnostics::Trace::Assert (m_BatterEvent || m_DestBatter == 0);

    if (m_DestBatter == 1 && m_DestRunner1 == 1) m_DestRunner1 = 2;
    if (m_DestBatter == 2 && m_DestRunner1 == 2) System::Diagnostics::Debug::Assert (false);
    if (m_DestBatter == 3 && m_DestRunner1 == 3) System::Diagnostics::Debug::Assert (false);
    if (m_DestBatter == 2 && m_DestRunner2 == 2) System::Diagnostics::Debug::Assert (false);
    if (m_DestBatter == 3 && m_DestRunner2 == 3) System::Diagnostics::Debug::Assert (false);
    if (m_DestBatter == 3 && m_DestRunner3 == 3) System::Diagnostics::Debug::Assert (false);
    if (m_DestRunner1 == 2 && m_DestRunner2 == 2) m_DestRunner2 = 3;
    if (m_DestRunner1 == 3 && m_DestRunner2 == 3) System::Diagnostics::Debug::Assert (false);
    if (m_DestRunner1 == 3 && m_DestRunner3 == 3) System::Diagnostics::Debug::Assert (false);
    if (m_DestRunner2 == 3 && m_DestRunner3 == 3) System::Diagnostics::Debug::Assert (false);

    if (m_BatterEvent && m_DestBatter == 0)
    {
        m_DestBatter--;
    }
    System::Byte StartRunners = 1 +
                                (m_1B != -1) +
                                (m_2B != -1) +
                                (m_3B != -1);
    System::Byte EndRunners =  (m_DestBatter >= 0 && m_DestBatter < 4) +
                               (m_DestRunner1 > 0 && m_DestRunner1 < 4) +
                               (m_DestRunner2 > 0 && m_DestRunner2 < 4) +
                               (m_DestRunner3 > 0 && m_DestRunner3 < 4);
    System::Byte RunsScored =   (m_DestBatter > 3) +
                                (m_DestRunner1 > 3) +
                                (m_DestRunner2 > 3) +
                                (m_DestRunner3 > 3);
    if ((m_DestBatter == 1) && (m_DestRunner1 == 1))
    {
        // offending event:
        //""WAS201108200","PHI",2,0,1,1,2,"BFFX",0,0,"valdw001","R","valdw001","R","lannj001","L","lannj001","L","ramow001","morsm001","espid001","zimmr001","desmi001","nix-l001","ankir001","wertj001","ruizc001","maybj001","howar001","5(2)2(3)/GDP","F","F",5,8,2,"T","T",0,"F","F",2,"T","F",0,"F","F",5,"G","F","F","",0,0,"N",0,"N",0,"N",1,1,0,0,"","","5","52","F","F","F","F","F","F","F","F","F","lannj001","lannj001","lannj001","F","F","F","F","F","","","","",0,5,2,0,5,0,0,0,0,13"
        m_DestRunner1 = 2;
    }
    m_RunnersLost = StartRunners - EndRunners - RunsScored - m_OutsOnPlay;
    //if (RunsScored > 0) System::Console::WriteLine (m_EventNum + "\t" + RunsScored);
    if (m_RunnersLost && (m_3B != -1) && (m_DestRunner3 == 0)) {m_DestRunner3 = 3; m_RunnersLost--; EndRunners++;}
    if (m_RunnersLost && (m_2B != -1) && (m_DestRunner2 == 0)) {m_DestRunner2 = 2; m_RunnersLost--; EndRunners++;}
    if (m_RunnersLost && (m_1B != -1) && (m_DestRunner1 == 0)) {m_DestRunner1 = 1; m_RunnersLost--; EndRunners++;}
    // if LostRunners > 0 then this is a stolen base
    System::Diagnostics::Trace::Assert (m_RunnersLost < 2);
    System::Diagnostics::Trace::Assert (m_OutsOnPlay ==
                                        (StartRunners - EndRunners - RunsScored - m_RunnersLost));
}

System::Byte Event::From (System::Void)
{
    System::Byte retval = 0;
    retval |= (m_Outs << 4);
    retval |= ((m_3B != -1)?0x08:00);
    retval |= ((m_2B != -1)?0x04:00);
    retval |= ((m_1B != -1)?0x02:00);
    retval |= 1;
    return retval;
}

System::Byte Event::To   (System::Void)
{
    System::Byte retval = 0;
    retval |= ((m_Outs + m_OutsOnPlay) << 4);
    retval |= ((m_DestBatter ==3 ||
                m_DestRunner1==3 ||
                m_DestRunner2==3 ||
                m_DestRunner3==3)?0x08:0x00);
    retval |= ((m_DestBatter ==2 ||
                m_DestRunner1==2 ||
                m_DestRunner2==2 ||
                m_DestRunner3==2)?0x04:0x00);
    retval |= ((m_DestBatter ==1 ||
                m_DestRunner1==1 ||
                m_DestRunner2==1 ||
                m_DestRunner3==1)?0x02:0x00);
    retval |= ((m_DestBatter ==0)?0x01:0x00);
    return retval;
}

Event::Event(System::String^ str,
             DBDictionary^   Players,
             DBDictionary^   Games,
             DBDictionary^   Teams,
             System::Boolean Full)
{
    array<System::Char>^splitChars = {','};
    array<System::Char>^trimChars = {'\"'};
    array<System::String^>^split = str->Split(splitChars);
    System::Diagnostics::Trace::Assert (( Full && split->Length == 97) ||
                                        (!Full && split->Length ==  9));
    System::Int32 i=0;
    /* -f 0-4,10,12,26-28,40,58-61 */
               m_Game             = Games->Add(split[i]->Trim(trimChars));    i++;
               m_Team             = Teams->Add(split[i]->Trim(trimChars));    i++;
               m_Inning           = System::Convert::ToByte (split[i]);       i++;
               m_HomeTeamBatting  = ZeroOneToBoolean(split[i]);               i++;
               m_Outs             = System::Convert::ToByte (split[i]);       i++;
    if (Full) {m_Balls            = System::Convert::ToByte (split[i]);       i++;
               m_Strikes          = System::Convert::ToByte (split[i]);       i++;
               m_Pitches          = split[i]->Trim(trimChars);                i++;
               m_ScoreVis         = System::Convert::ToByte (split[i]);       i++;
               m_ScoreHome        = System::Convert::ToByte (split[i]);       i++;}
               m_BatterStart      = Players->Add (split[i]->Trim(trimChars)); i++;
    if (Full) {m_BatsRightStart   = RLToBoolean(split[i]);                    i++;}
               m_BatterRes        = Players->Add (split[i]->Trim(trimChars)); i++;
    if (Full) {m_BatsRightRes     = RLToBoolean(split[i]);                    i++;
               m_PitcherStart     = Players->Add (split[i]->Trim(trimChars)); i++;
               m_PitchRightStart  = RLToBoolean(split[i]);                    i++;
               m_PitcherRes       = Players->Add (split[i]->Trim(trimChars)); i++;
               m_PitchRightRes    = RLToBoolean(split[i]);                    i++;
               m_Catcher          = Players->Add (split[i]->Trim(trimChars)); i++;
               m_First            = Players->Add (split[i]->Trim(trimChars)); i++;
               m_Second           = Players->Add (split[i]->Trim(trimChars)); i++;
               m_Third            = Players->Add (split[i]->Trim(trimChars)); i++;
               m_Shortstop        = Players->Add (split[i]->Trim(trimChars)); i++;
               m_Left             = Players->Add (split[i]->Trim(trimChars)); i++;
               m_Center           = Players->Add (split[i]->Trim(trimChars)); i++;
               m_Right            = Players->Add (split[i]->Trim(trimChars)); i++;}
               m_1B               = Players->Add (split[i]->Trim(trimChars)); i++;
               m_2B               = Players->Add (split[i]->Trim(trimChars)); i++;
               m_3B               = Players->Add (split[i]->Trim(trimChars)); i++;
    if (Full) {m_EventText        = split[i]->Trim(trimChars);                i++;
               m_Leadoff          = TFToBoolean(split[i]);                    i++;
               m_PinchHit         = TFToBoolean(split[i]);                    i++;
               m_DefensePos       = System::Convert::ToByte (split[i]);       i++;
               m_LineupPos        = System::Convert::ToByte (split[i]);       i++;
               m_EventType        = System::Convert::ToByte (split[i]);       i++;
               m_BatterEvent      = TFToBoolean(split[i]);                    i++;
               m_AB               = TFToBoolean(split[i]);                    i++;
               m_HitValue         = System::Convert::ToByte (split[i]);       i++;
               m_SH               = TFToBoolean(split[i]);                    i++;
               m_SF               = TFToBoolean(split[i]);                    i++;}
               m_OutsOnPlay       = System::Convert::ToByte (split[i]);       i++;
    if (Full) {m_DoublePlay       = TFToBoolean(split[i]);                    i++;
               m_TriplePlay       = TFToBoolean(split[i]);                    i++;
               m_RBI              = System::Convert::ToByte (split[i]);       i++;
               m_WildPitch        = TFToBoolean(split[i]);                    i++;
               m_PassedBall       = TFToBoolean(split[i]);                    i++;
               m_FieldPos         = System::Convert::ToByte (split[i]);       i++;
               m_BattedBall       = split[i]->Trim(trimChars);                i++;
               m_Bunt             = TFToBoolean(split[i]);                    i++;
               m_Foul             = TFToBoolean(split[i]);                    i++;
               m_HitLocation      = split[i]->Trim(trimChars);                i++;
               m_NumErrors        = System::Convert::ToByte (split[i]);       i++;
               m_Error1Player     = System::Convert::ToByte (split[i]);       i++;
               m_Error1Type       = split[i]->Trim(trimChars);                i++;
               m_Error2Player     = System::Convert::ToByte (split[i]);       i++;
               m_Error2Type       = split[i]->Trim(trimChars);                i++;
               m_Error3Player     = System::Convert::ToByte (split[i]);       i++;
               m_Error3Type       = split[i]->Trim(trimChars);                i++;}
               m_DestBatter       = System::Convert::ToByte (split[i]);       i++;
               m_DestRunner1      = System::Convert::ToByte (split[i]);       i++;
               m_DestRunner2      = System::Convert::ToByte (split[i]);       i++;
               m_DestRunner3      = System::Convert::ToByte (split[i]);       i++;
    if (Full) {m_PlayBatter       = split[i]->Trim(trimChars);                i++;
               m_PlayRunner1      = split[i]->Trim(trimChars);                i++;
               m_PlayRunner2      = split[i]->Trim(trimChars);                i++;
               m_PlayRunner3      = split[i]->Trim(trimChars);                i++;
               m_SBRunner1        = TFToBoolean(split[i]);                    i++;
               m_SBRunner2        = TFToBoolean(split[i]);                    i++;
               m_SBRunner3        = TFToBoolean(split[i]);                    i++;
               m_CSRunner1        = TFToBoolean(split[i]);                    i++;
               m_CSRunner2        = TFToBoolean(split[i]);                    i++;
               m_CSRunner3        = TFToBoolean(split[i]);                    i++;
               m_PORunner1        = TFToBoolean(split[i]);                    i++;
               m_PORunner2        = TFToBoolean(split[i]);                    i++;
               m_PORunner3        = TFToBoolean(split[i]);                    i++;
               m_PitcherResp1     = Players->Add (split[i]->Trim(trimChars)); i++;
               m_PitcherResp2     = Players->Add (split[i]->Trim(trimChars)); i++;
               m_PitcherResp3     = Players->Add (split[i]->Trim(trimChars)); i++;
               m_GameNew          = TFToBoolean(split[i]);                    i++;
               m_GameEnd          = TFToBoolean(split[i]);                    i++;
               m_Pinch1           = TFToBoolean(split[i]);                    i++;
               m_Pinch2           = TFToBoolean(split[i]);                    i++;
               m_Pinch3           = TFToBoolean(split[i]);                    i++;
               m_Runner1Removed   = Players->Add (split[i]->Trim(trimChars)); i++;
               m_Runner2Removed   = Players->Add (split[i]->Trim(trimChars)); i++;
               m_Runner3Removed   = Players->Add (split[i]->Trim(trimChars)); i++;
               m_BatterRemoved    = Players->Add (split[i]->Trim(trimChars)); i++;
               m_BatterRemovedPos = System::Convert::ToByte (split[i]);       i++;
               m_FielderPO1       = System::Convert::ToByte (split[i]);       i++;
               m_FielderPO2       = System::Convert::ToByte (split[i]);       i++;
               m_FielderPO3       = System::Convert::ToByte (split[i]);       i++;
               m_FielderAst1      = System::Convert::ToByte (split[i]);       i++;
               m_FielderAst2      = System::Convert::ToByte (split[i]);       i++;
               m_FielderAst3      = System::Convert::ToByte (split[i]);       i++;
               m_FielderAst4      = System::Convert::ToByte (split[i]);       i++;
               m_FielderAst5      = System::Convert::ToByte (split[i]);       i++;
               m_EventNum         = System::Convert::ToByte (split[i]);       i++;}
}

System::String^ Event::Dump(System::Void)
{
    return From().ToString() + "," + To().ToString();
}
