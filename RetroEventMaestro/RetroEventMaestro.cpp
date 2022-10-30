// RetroEventMaestro.cpp : main project file.

#include "stdafx.h"

using namespace System;
#define NUMRUNNERS 8
//#define NUMRUNNERS 16
//#define SKIPHOME

struct GameState
{
    System::Boolean HomeAway;
    System::Byte Outs;
    System::Byte Balls;
    System::Byte Strikes;
    System::Boolean Third;
    System::Boolean Second;
    System::Boolean First;
    System::Boolean Home;
    System::Int16 ToInt16(void);
    GameState(void);
    GameState(array<System::String^>^ arr);
    System::Boolean addStrike(void);
    System::Boolean addFoul (void);
    System::Boolean addBall (void);
    System::Void SetNewInning(System::Boolean HA);
    System::Void SetNewAB(void);
};
System::Void GameState::SetNewInning(System::Boolean HA)
{
    HomeAway = HA;
    Outs     = 0;
    Balls    = 0;
    Strikes  = 0;
    Third    = false;
    Second   = false;
    First    = false;
    Home     = false;
}
System::Void GameState::SetNewAB(void)
{
    Balls    = 0;
    Strikes  = 0;
}
System::Boolean GameState::addStrike(void)
{
    System::Boolean retval = Strikes == 2;
    if (!retval) Strikes++;
    return retval;
}
System::Boolean GameState::addFoul (void)
{
    if (Strikes < 2) Strikes++;
    return false;
}
System::Boolean GameState::addBall (void)
{
    System::Boolean retval = Balls == 3;
    if (!retval) Balls++;
    return retval;
}
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
    eventText, // unused
    batterEventFlag,
    outsOnPlay,
    batterDest,
    runnerOn1stDest,
    runnerOn2ndDest,
    runnerOn3rdDest,
    newGameFlag,
    NumEnums
};
GameState::GameState(array<System::String^>^ arr)
{
    System::Diagnostics::Debug::Assert(arr->Length == NumEnums);
    HomeAway = arr[battingTeam]=="1";
    Outs     = System::Convert::ToByte(arr[outs]);
    Balls    = System::Convert::ToByte(arr[balls]);
    Strikes  = System::Convert::ToByte(arr[strikes]);
    Home     = true;
    First    = arr[firstRunner ]->Trim('"') != "";
    Second   = arr[secondRunner]->Trim('"') != "";
    Third    = arr[thirdRunner ]->Trim('"') != "";
}
System::Int16 GameState::ToInt16(void)
{
    return (Outs << 8)        |
           (Third ?0x80:0x00) |
           (Second?0x40:0x00) |
           (First ?0x20:0x00) |
           (Home  ?0x10:0x00) |
           (Balls << 2)       |
           (Strikes);
}
GameState::GameState(void)
{
    HomeAway = true;
    Outs     = 0;
    Balls    = 0;
    Strikes  = 0;
    Third    = false;
    Second   = false;
    First    = false;
    Home     = false;
}
System::Int32 Freq[1024][1024] = {0};
System::Int32 Runs[1024] = {0};
System::Double Values[768] = {0};  // no need for 3 outs for output

void ProcessTeam (System::String^ team);
#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))
// Debug\RetroEventMaestro.exe 2018eve\evs.txt > evs.out
// Debug\RetroEventMaestro.exe results.txt > results.out
System::Int32 PlayerVal(System::Int32 v);
  const System::Byte CountOrder[] = {0x2, 0x6, 0x1, 0xA, 0x5, 0x0, 0x4, 0x9, 0xE, 0x8, 0xD, 0xC}; // tango order
//const System::Byte CountOrder[] = {0x2, 0x6, 0xA, 0x1, 0x5, 0xE, 0x0, 0x9, 0x4, 0xD, 0x8, 0xC}; // better order?
int main(array<System::String ^> ^args)
{
    System::IO::TextReader^ rdr = gcnew System::IO::StreamReader(args[0]);
    System::String^ line;
    //goto next;
    while ((line = rdr->ReadLine()) != nullptr)
    {
        ProcessTeam(line);
    }
    rdr->Close();
    for (System::Int32 i=0; i<1024; i++)
    {
        System::Diagnostics::Debug::Assert (Freq[i][i]==0);
        System::Int32 sum = 0;
        for (System::Int32 j=0; j<1024; j++)
        {
            if (Freq[i][j]>0)
            {
                System::Int32 runValue = MAX(PlayerVal(i>>4) - PlayerVal(j>>4),0);
                Runs[i] -= runValue * Freq[i][j];
                sum -= Freq[i][j];
                //System::Console::WriteLine("Freq["+i.ToString("X3")+"]["+j.ToString("X3")+"] -> "+Freq[i][j]);
                System::Console::WriteLine(i + "," + j + "," + Freq[i][j]);
            }
        }
        System::Diagnostics::Debug::Assert(Freq[i][i] == 0);
        //Freq[i][i] = MIN(sum,-1); -- moved to R
        if (sum != 0)
        {
            Freq[i][i] = sum;
            System::Console::WriteLine(i + "," + i + "," + Freq[i][i]);
        }
    }
    //for (System::Int32 i=0; i<256; i++)
    //    for (System::Int32; 
    //{
    //    Freq[i][
    //}
    for (System::Int32 i=0; i<1024; i++)
    {
        System::Console::WriteLine(i + "," + Runs[i] + ",0");
    }
    //System::Diagnostics::Debug::WriteLine ("PlayerVal(250) = " + PlayerVal(250>>4));
goto end;
next:
    System::Int32 j=0;
    while ((line = rdr->ReadLine()) != nullptr)
    {
        //array<System::String^>^ split = line->Split(gcnew array<wchar_t>{'[',']'});
        //System::Diagnostics::Debug::Assert (split->Length == 3);
        //System::Int32 idx = System::Convert::ToInt32(split[1])-1;
        //System::Double val = System::Convert::ToDouble(split[2]);
        //Values[idx] = val;
        array<System::Char>^sep = gcnew array<System::Char>{' '};
        array<System::String^>^ split = line->Split(sep, System::StringSplitOptions::RemoveEmptyEntries);
        //System::Diagnostics::Debug::Assert();
        for (System::Int32 i=1; i<split->Length; i++, j++)
        {
            System::Double val = System::Convert::ToDouble(split[i]);
            Values[j] = val;
        }
        //linecount++;
    }
    rdr->Close();
    for (System::Int32 count = 0; count<12; count++)
    {
        System::Console::Write((CountOrder[count]>>2) + "-" + (CountOrder[count]&3) + "\t");
    }
    System::Console::WriteLine();
    for (System::Int32 outs = 0; outs<3; outs++)
    {
        for (System::Int32 runners=0; runners<NUMRUNNERS; runners++)
        {
            for (System::Int32 count = 0; count<12; count++)
            {
#if NUMRUNNERS==8
                System::Int32 idx = (outs<<8) + (runners<<5) + 0x10 + CountOrder[count];
#elif NUMRUNNERS==16
                System::Int32 idx = (outs<<8) + (runners<<4)        + CountOrder[count];
#else
#error
#endif
                //System::Diagnostics::Debug::WriteLine ("idx=" + idx.ToString("X3"));
                System::Diagnostics::Debug::Assert(NUMRUNNERS==16 || Values[idx] > 0.);
                System::Console::Write(Values[idx].ToString("F") + L"\t");
            }
#if NUMRUNNERS==8
            System::Console::Write    ((runners&1)?"1B ":"-- ");
            System::Console::Write    ((runners&2)?"2B ":"-- ");
            System::Console::WriteLine((runners&4)?"3B ":"-- ");
#elif NUMRUNNERS==16
            System::Console::Write    ((runners&1)?"H  ":"-- ");
            System::Console::Write    ((runners&2)?"1B ":"-- ");
            System::Console::Write    ((runners&4)?"2B ":"-- ");
            System::Console::WriteLine((runners&8)?"3B ":"-- ");
#else
#error
#endif
        }
        System::Console::WriteLine();
    }
end:
    return 0;
}

void ProcessTeam (System::String^ team)
{
    struct GameState state;
    System::IO::TextReader^ rdr = gcnew System::IO::StreamReader(team);
    System::String^ line;
    System::Boolean EndOfAB = false;
    System::Int16 StateVal = -1;
    System::Int32 count = 0;
//System::Int32 FreqPerState[1024];
/*
    for (System::Int32 i=0; i<1024; i++)
    {
        FreqPerInning[i].count = 0;
        FreqPerInning[i].runs = 0;
    }
*/
    while ((line = rdr->ReadLine()) != nullptr)
    {
        count++;
        array<System::String^>^ split = line->Split(',');
        GameState newState(split);
        if ((newState.HomeAway != state.HomeAway) ||
            (split[newGameFlag]->Trim('"') == "T"))
        {
            state.SetNewInning(newState.HomeAway);
            //System::Diagnostics::Debug::Assert (split[newGameFlag]->Trim('"') == "T" ||
            //                                    StateVal != state.ToInt16());
            //System::Diagnostics::Debug::WriteLine(StateVal.ToString("X3"));
            StateVal = state.ToInt16();
            //System::Diagnostics::Debug::WriteLine(StateVal.ToString("X3"));
        }
        state.Home = true;
        System::String^ pitches = split[pitchSequence]->Trim('"');
        System::Int32 lastperiod = pitches->LastIndexOf(".");
        if (lastperiod != -1)
        {
            pitches = pitches->Substring(lastperiod+1);
        }
        for each (System::Char c in pitches)
        {
            if (StateVal != state.ToInt16())
            {
                Freq[StateVal][state.ToInt16()]++;
                StateVal = state.ToInt16();
                //System::Diagnostics::Debug::WriteLine(StateVal.ToString("X3"));
            }
            switch (c)
            {
            case 'V':                                                   // called ball because pitcher went to his mouth
            case 'P':                                                   // pitchout
            case 'I':                                                   // intentional ball
            case 'B': EndOfAB = state.addBall();                 break; // ball
            case 'M': //System::Diagnostics::Debug::WriteLine(line);    // missed bunt attempt
            case 'O': //System::Diagnostics::Debug::WriteLine(line);    // foul tip on bunt
            case 'L': //System::Diagnostics::Debug::WriteLine(line);    // foul bunt
            case 'T': //System::Diagnostics::Debug::WriteLine(line);    // foul tip
            case 'S':                                                   // swinging strike
            case 'K':                                                   // strike (unknown type)
            case 'Q':                                                   // swinging on pitchout
            case 'C': EndOfAB = state.addStrike();               break; // called strike
            case 'R':                                                   // foul ball on pitchout
            case 'F': EndOfAB = state.addFoul();                 break; // foul
            case 'Y':                                                   // ball put into play on pitchout
            case 'H':                                                   // hit batter
            case 'X': EndOfAB = true;                            break; // ball put into play by batter
            case 'D': //System::Diagnostics::Debug::WriteLine(line);    // not defined by Retrosheet - looks like catcher's interference
            case '1':                                                   // pickoff throw to first
            case '2':                                                   // pickoff throw to second
            case '3':                                                   // pickoff throw to third
            case '>':                                                   // Indicates a runner going on the pitch
            case '+':                                                   // following pickoff throw by the catcher
            case 'N':                                                   // no pitch (on balks and interference calls)
            case '*':                                            break; // indicates the following pitch was blocked by the catcher
            case 'U':                                                   // unknown or missed pitch
            case '.':                                                   // marker for play not involving the batter
            default: System::Diagnostics::Debug::Assert (false); break;
            }
        }
        state.First  = split[batterDest] == "1" || split[runnerOn1stDest] == "1";
        state.Second = split[batterDest] == "2" || split[runnerOn1stDest] == "2" || split[runnerOn2ndDest] == "2";
        state.Third  = split[batterDest] == "3" || split[runnerOn1stDest] == "3" || split[runnerOn2ndDest] == "3" || split[runnerOn3rdDest] == "3";
        state.Home = true;
        state.Outs = newState.Outs + System::Convert::ToByte(split[outsOnPlay]);
        if (split[batterEventFlag]->Trim('"') == "T")
        {
            state.Home = false;
            state.SetNewAB();
            System::Diagnostics::Debug::Assert (StateVal != state.ToInt16());
            Freq[StateVal][state.ToInt16()]++;
            StateVal = state.ToInt16();
            //System::Diagnostics::Debug::WriteLine(StateVal.ToString("X3"));
            EndOfAB = false;
        }
    }
    rdr->Close();
}
System::Int32 PlayerVal(System::Int32 v)
{
    System::Int32 retval = v>>4;
    retval += v&1?1:0;
    retval += v&2?1:0;
    retval += v&4?1:0;
    retval += v&8?1:0;
    return retval;
}