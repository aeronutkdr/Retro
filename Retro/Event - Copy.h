#pragma once
#include "StringList.h"

namespace Retro
{
    public ref class Event
    {
    protected:
        System::Int32   m_Game;
        System::Int32   m_Team;
        System::Byte    m_Inning;
        System::Boolean m_HomeTeamBatting;
        System::Byte    m_Outs;
        System::Byte    m_Balls;
        System::Byte    m_Strikes;
        System::String^ m_Pitches;
        System::Byte    m_ScoreVis;
        System::Byte    m_ScoreHome;
        System::Int32   m_BatterStart;
        System::Boolean m_BatsRightStart;
        System::Int32   m_BatterRes;
        System::Boolean m_BatsRightRes;
        System::Int32   m_PitcherStart;
        System::Boolean m_PitchRightStart;
        System::Int32   m_PitcherRes;
        System::Boolean m_PitchRightRes;
        System::Int32   m_Catcher;
        System::Int32   m_First;
        System::Int32   m_Second;
        System::Int32   m_Third;
        System::Int32   m_Shortstop;
        System::Int32   m_Left;
        System::Int32   m_Center;
        System::Int32   m_Right;
        System::Int32   m_1B;
        System::Int32   m_2B;
        System::Int32   m_3B;
        System::String^ m_EventText;
        System::Boolean m_Leadoff;
        System::Boolean m_PinchHit;
        System::Byte    m_DefensePos;
        System::Byte    m_LineupPos;
        System::Byte    m_EventType;
        System::Boolean m_BatterEvent;
        System::Boolean m_AB;
        System::Byte    m_HitValue;
        System::Boolean m_SH;
        System::Boolean m_SF;
        System::Byte    m_OutsOnPlay;
        System::Boolean m_DoublePlay;
        System::Boolean m_TriplePlay;
        System::Byte    m_RBI;
        System::Boolean m_WildPitch;
        System::Boolean m_PassedBall;
        System::Byte    m_FieldPos;
        System::String^ m_BattedBall;
        System::Boolean m_Bunt;
        System::Boolean m_Foul;
        System::String^ m_HitLocation;
        System::Byte    m_NumErrors;
        System::Byte    m_Error1Player;
        System::String^ m_Error1Type;
        System::Byte    m_Error2Player;
        System::String^ m_Error2Type;
        System::Byte    m_Error3Player;
        System::String^ m_Error3Type;
        System::Byte    m_DestBatter;
        System::Byte    m_DestRunner1;
        System::Byte    m_DestRunner2;
        System::Byte    m_DestRunner3;
        System::String^ m_PlayBatter;
        System::String^ m_PlayRunner1;
        System::String^ m_PlayRunner2;
        System::String^ m_PlayRunner3;
        System::Boolean m_SBRunner1;
        System::Boolean m_SBRunner2;
        System::Boolean m_SBRunner3;
        System::Boolean m_CSRunner1;
        System::Boolean m_CSRunner2;
        System::Boolean m_CSRunner3;
        System::Boolean m_PORunner1;
        System::Boolean m_PORunner2;
        System::Boolean m_PORunner3;
        System::Int32   m_PitcherResp1;
        System::Int32   m_PitcherResp2;
        System::Int32   m_PitcherResp3;
        System::Boolean m_GameNew;
        System::Boolean m_GameEnd;
        System::Boolean m_Pinch1;
        System::Boolean m_Pinch2;
        System::Boolean m_Pinch3;
        System::Int32   m_Runner1Removed;
        System::Int32   m_Runner2Removed;
        System::Int32   m_Runner3Removed;
        System::Int32   m_BatterRemoved;
        System::Byte    m_BatterRemovedPos;
        System::Byte    m_FielderPO1;
        System::Byte    m_FielderPO2;
        System::Byte    m_FielderPO3;
        System::Byte    m_FielderAst1;
        System::Byte    m_FielderAst2;
        System::Byte    m_FielderAst3;
        System::Byte    m_FielderAst4;
        System::Byte    m_FielderAst5;
        System::Byte    m_EventNum;

        System::Boolean TFToBoolean      (System::String ^ str);
        System::Boolean RLToBoolean      (System::String ^ str);
        System::Boolean ZeroOneToBoolean (System::String ^ str);
    public:
        Event(System::String^ str,
              StringList^     Players,
              StringList^     Games,
              StringList^     Teams);
        System::Byte From (System::Void);
        System::Byte To   (System::Void);
    };
}