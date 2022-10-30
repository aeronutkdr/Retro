#include "stdafx.h"
#include "GameState.h"

GameState::GameState(void)
{
    SetNewInning(true);
}
System::Void GameState::SetNewInning(System::Boolean HA)
{
    HomeAway       = HA;
    Outs           = 0;
    Balls          = 0;
    Strikes        = 0;
    Fouls2Strikes  = 0;
    Third          = false;
    Second         = false;
    First          = false;
    Home           = false;
    LastPitch      = nullptr;
}

System::Void GameState::SetNewAB(void)
{
    Balls         = 0;
    Strikes       = 0;
    Fouls2Strikes = 0;
    Home          = true;
    LastPitch     = nullptr;
}
System::Void GameState::addStrike(void)
{
    //System::Boolean retval = Strikes == 2;
    //if (!retval) Strikes++;
    Strikes++;
}
System::Void GameState::addFoul (void)
{
    if (Strikes < 2) Strikes++;
    else Fouls2Strikes++;
}
System::Void GameState::addBall (void)
{
    Balls++;
}
System::Void GameState::ProcessEvent(array<System::String^>^ split, array<System::Int32, 2>^ freq,
                                     System::Collections::Generic::Dictionary<System::Int32, System::Int32>^ dict)
{
    System::Diagnostics::Debug::Assert(split->Length == NumEnums);
    System::Boolean HA = split[battingTeam]=="1";
    if ((split[newGameFlag]->Trim('"') == "T") ||
        (HomeAway != HA))
    {
        SetNewInning(HA);
        //System::Diagnostics::Debug::WriteLine("NEW INNING");
    }
System::Int16 StateVal = ToInt16();
    if (!Home)
    {
        SetNewAB();
    }
    //System::Int16 StateVal = ToInt16();
    System::Int16 newval = ToInt16();
    if (newval != StateVal)
    {
        //System::Diagnostics::Debug::WriteLine (StateVal.ToString("X3") + " -> " + newval.ToString("X3"));
        freq[StateVal, newval]++;
        if (dict->ContainsKey(StateVal << 16 | newval)) dict[StateVal << 16 | newval]++;
        else                                            dict->Add(StateVal << 16 | newval,1);
        StateVal = newval;
    }
    System::Diagnostics::Debug::Assert ((split[firstRunner ]->Trim('"') != "") == First);
    System::Diagnostics::Debug::Assert ((split[secondRunner]->Trim('"') != "") == Second);
    System::Diagnostics::Debug::Assert ((split[thirdRunner ]->Trim('"') != "") == Third);
    System::Diagnostics::Debug::Assert(Outs==System::Convert::ToByte(split[outs]));
    System::String^ pitches = split[pitchSequence]->Trim('"');
    if (LastPitch != nullptr)
    {
        /* can't check == 0:
        0,2,0,0,">C","davir003","","","SB2","F",0,0,2,0,0,"F"
        0,2,2,1,">C.B*BX","","davir003","","7/L","T",1,0,0,2,0,"F"
        */
        System::Int32 loc = pitches->LastIndexOf('.');
        /* this is for: 2018HOU.out line 5338 */
        pitches = pitches->Replace("^","");
        if (pitches->IndexOf(LastPitch) != -1)
        {
            pitches = pitches->Substring(LastPitch->Length);
        }
        else
        {
            System::Diagnostics::Debug::Assert (loc != -1);
            pitches = pitches->Substring(loc+1);
        }
    }
    System::Boolean b = pitches->Length == 1;
    for (System::Int32 i = 0; i<pitches->Length-1; i++)
    {
        b = ProcessPitch(pitches[i]) || b;
        newval = ToInt16();
        if (newval != StateVal)
        {
            //System::Diagnostics::Debug::WriteLine (StateVal.ToString("X3") + " -> " + newval.ToString("X3"));
            freq[StateVal, newval]++;
            if (dict->ContainsKey(StateVal << 16 | newval)) dict[StateVal << 16 | newval]++;
            else                                            dict->Add(StateVal << 16 | newval,1);
            StateVal = newval;
        }
    }
    System::Diagnostics::Debug::Assert(Balls==System::Convert::ToByte(split[balls]));
    System::Diagnostics::Debug::Assert(Strikes==System::Convert::ToByte(split[strikes]));
    if ((b || (split[batterEventFlag]->Trim('"') == "F")) && (pitches->Length>0))
    {
        b = ProcessPitch(pitches[pitches->Length-1]);
    }
    Outs += System::Convert::ToByte(split[outsOnPlay]);
    System::Diagnostics::Debug::Assert (Outs <= 3);
    Home = split[batterEventFlag]->Trim('"') == "F";
    if (Home)
    {
        LastPitch += pitches;
    }
    else
    {
        LastPitch = nullptr;
    }
    First  = split[batterDest] == "1" || split[runnerOn1stDest] == "1";
    Second = split[batterDest] == "2" || split[runnerOn1stDest] == "2" || split[runnerOn2ndDest] == "2";
    Third  = split[batterDest] == "3" || split[runnerOn1stDest] == "3" || split[runnerOn2ndDest] == "3" || split[runnerOn3rdDest] == "3";
    newval = ToInt16();
    if (newval != StateVal)
    {
        //System::Diagnostics::Debug::WriteLine (StateVal.ToString("X3") + " -> " + newval.ToString("X3"));
        freq[StateVal, newval]++;
        if (dict->ContainsKey(StateVal << 16 | newval)) dict[StateVal << 16 | newval]++;
        else                                            dict->Add(StateVal << 16 | newval,1);
        StateVal = newval;
    }
}

System::Boolean GameState::ProcessPitch(System::Char c)
{
    System::Boolean retval = false;
    switch (c)
    {
    case 'V':                                                   // called ball because pitcher went to his mouth
    case 'P':                                                   // pitchout
    case 'I':                                                   // intentional ball
    case 'B': addBall();                                 break; // ball

    case 'M':                                                   // missed bunt attempt
    case 'O':                                                   // foul tip on bunt
    case 'L':                                                   // foul bunt
    case 'T':                                                   // foul tip
    case 'S':                                                   // swinging strike
    case 'K':                                                   // strike (unknown type)
    case 'Q':                                                   // swinging on pitchout
    case 'C': addStrike();                               break; // called strike

    case 'R':                                                   // foul ball on pitchout
    case 'F': addFoul();                                 break; // foul

    case 'Y':                                                   // ball put into play on pitchout
    case 'H':                                                   // hit batter
    case 'X':                                            break; // ball put into play by batter

    case '>': retval=true;                               break; // Indicates a runner going on the pitch

    case '.':                                                   // marker for play not involving the batter
    case 'D':                                                   // not defined by Retrosheet - looks like catcher's interference
    case '1':                                                   // pickoff throw to first
    case '2':                                                   // pickoff throw to second
    case '3':                                                   // pickoff throw to third
    case '+':                                                   // following pickoff throw by the catcher
    case 'N':                                                   // no pitch (on balks and interference calls)
    case '*':                                            break; // indicates the following pitch was blocked by the catcher

    case 'U':                                                   // unknown or missed pitch
    default: System::Diagnostics::Debug::Assert (false); break;
    }
    return retval;
}

System::Int16 GameState::ToInt16(void)
{
    // limit balls to 3
    System::Byte b = Balls   > 3?3:Balls;
    System::Byte s = Strikes > 2?2:Strikes;
    return (Outs << 8)        |
           (Third ?0x80:0x00) |
           (Second?0x40:0x00) |
           (First ?0x20:0x00) |
           (Home  ?0x10:0x00) |
           (b << 2)           |
           (s);
}

#if 0
GameState::GameState(array<System::String^>^ arr)
{
    System::Diagnostics::Debug::Assert(arr->Length == NumEnums);
    HomeAway = arr[battingTeam]=="1";
    Outs     = System::Convert::ToByte(arr[outs]);
    if (!Home) SetNewAB();
    Home     = true;
    First    = arr[firstRunner]->Trim('"') != "";
    Second   = arr[secondRunner]->Trim('"') != "";
    Third    = arr[thirdRunner]->Trim('"') != "";
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

System::Int32 GameState::RunsFrom(System::Int16 from)
{
    System::Int32 retval = (from>>8) +
                           ((from&0x80)?1:0)+
                           ((from&0x40)?1:0)+
                           ((from&0x20)?1:0)+
                           ((from&0x10)?1:0);
    retval -= Outs;
    retval -= Third?1:0;
    retval -= Second?1:0;
    retval -= First?1:0;
    retval -= Home?1:0;
    return retval;
}

System::Void GameState::removeStrike(void)
{
    System::Diagnostics::Debug::Assert (Strikes > 0);
    System::Diagnostics::Debug::Assert (Fouls2Strikes == 0);
    Strikes--;
}
System::Void GameState::removeFoul (void)
{
    System::Diagnostics::Debug::Assert (Strikes > 0);
    if ((Strikes < 2) ||
        (Fouls2Strikes == 0))
        Strikes--;
    else if (Fouls2Strikes > 0) Fouls2Strikes--;
}
System::Void GameState::removeBall (void)
{
    System::Diagnostics::Debug::Assert (Balls > 0);
    Balls--;
}

System::Boolean GameState::Apply(array<System::String^>^ arr)
{
    System::Diagnostics::Debug::Assert(arr->Length == NumEnums);
    System::Diagnostics::Debug::Assert(Outs==System::Convert::ToByte(arr[outs]));
    HomeAway = arr[battingTeam]=="1";
    Outs     = System::Convert::ToByte(arr[outs]);
    if (!Home) SetNewAB();
    Home     = true;
    First    = arr[firstRunner]->Trim('"') != "";
    Second   = arr[secondRunner]->Trim('"') != "";
    Third    = arr[thirdRunner]->Trim('"') != "";
    return true;
}
#endif