#ifndef GAMESTATE_H
#define GAMESTATE_H
#include "stdafx.h"

enum EventFieldType
{
    battingTeam,
    outs,
    balls,
    strikes,
    pitchSequence,
    firstRunner,
    secondRunner,
    thirdRunner,
    eventText,
    batterEventFlag,
    outsOnPlay,
    batterDest,
    runnerOn1stDest,
    runnerOn2ndDest,
    runnerOn3rdDest,
    newGameFlag,
    NumEnums
};

ref class GameState
{
public:
    GameState(void);
    System::Void ProcessEvent(array<System::String^>^ split, array<System::Int32, 2>^ freq,
        System::Collections::Generic::Dictionary<System::Int32, System::Int32>^ dict);
protected:
    System::String^ LastPitch;
    System::Boolean HomeAway;
    System::Byte Outs;
    System::Byte Balls;
    System::Byte Strikes;
    System::Byte Fouls2Strikes;
    System::Boolean Third;
    System::Boolean Second;
    System::Boolean First;
    System::Boolean Home;
protected:
    System::Void SetNewInning(System::Boolean HA);
    System::Void SetNewAB(void);
    System::Boolean ProcessPitch(System::Char c);
    System::Void addStrike(void);
    System::Void addFoul (void);
    System::Void addBall (void);
    System::Int16 ToInt16(void);
#if 0
    GameState(array<System::String^>^ arr);
    System::Boolean Apply(array<System::String^>^ arr);
    System::Int32 RunsFrom(System::Int16 from);
    System::Void removeStrike(void);
    System::Void removeFoul (void);
    System::Void removeBall (void);
#endif
};

#endif /* GAMESTATE_H */