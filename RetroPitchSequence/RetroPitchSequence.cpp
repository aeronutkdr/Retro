// RetroPitchSequence.cpp : main project file.
#include "stdafx.h"
using namespace System;
System::Collections::Generic::List<System::Int16>^ GenSequence (System::Int16 start,
                                                                System::String^ pitches,
                                                                System::String^ events);
//System::Collections::Generic::List<System::Int16>^ GenSequence2 (System::Int16 start,
//                                                                 System::String^ pitches,
//                                                                 System::String^ events);
System::Int16 ToInt16 (System::Byte Outs,
                       System::Boolean Third,
                       System::Boolean Second,
                       System::Boolean First,
                       System::Boolean Home,
                       System::Byte Balls,
                       System::Byte Strikes);
System::Boolean ParseGame (System::IO::TextReader^ rdr,
                           array<System::UInt32, 2>^ trans);
#if 0
System::Byte ParseInning (System::IO::TextReader^ rdr,
                          array<System::UInt32, 2>^ trans);
#endif
System::Int32 ParseEvent (System::String^ evt, System::Int16* start);
//System::Int32 ParseEvent2 (System::String^ evt, System::Int16* start);

int main(array<System::String ^> ^args)
{
    array<System::UInt32, 2>^ transitions = gcnew array<System::UInt32, 2>(1024,1024);
    transitions->Initialize();
    System::IO::TextReader ^rdr = gcnew System::IO::StreamReader(args[0]);
    while (rdr->ReadLine()->IndexOf("id,") != 0);
    while (!ParseGame(rdr, transitions));
    rdr->Close();
    return 0;
}

System::Collections::Generic::List<System::Int16>^ GenSequence (System::Int16 start,
                                                                System::String^ pitches,
                                                                System::String^ events)
{
    //https://www.retrosheet.org/eventfile.htm#3
    System::Collections::Generic::List<System::Int16>^ retval =
        gcnew System::Collections::Generic::List<System::Int16>();
    // must start with a 0-0 count
    System::Diagnostics::Debug::Assert ((start & 0x0F) == 0);
    //retval->Add(start);
    System::Boolean endofAB = false;
    System::Int32 eventidx = 0;
    System::Int32 i;
    for (i=0; i<pitches->Length && !endofAB; i++)
    {
        switch (pitches[i])
        {
        case '*'://  indicates the following pitch was blocked by the catcher
        case '1'://  pickoff throw to first
        case '2'://  pickoff throw to second
        case '3'://  pickoff throw to third
        case '+'://  following pickoff throw by the catcher
            // no real change
            break;
        case '>'://  Indicates a runner going on the pitch
            eventidx += ParseEvent(events->Substring(eventidx), &start);
            break;
        case '.'://  marker for play not involving the batter
            break;
        case 'B'://  ball
        case 'I'://  intentional ball
        case 'M'://  missed bunt attempt
        case 'V'://  called ball because pitcher went to his mouth
        case 'P'://  pitchout
            {
                System::Byte balls = (System::Byte) (start>>2 & 0x03);
                endofAB = balls == 3;
                if (!endofAB)
                {   balls++;
                    start &= 0x3F3u;
                    start |= (System::Int16) (balls << 2);
                    retval->Add(start);
                }
            }
            break;
        case 'C'://  called strike
        case 'K'://  strike (unknown type)
        case 'S'://  swinging strike
        case 'F'://  foul
        case 'L'://  foul bunt
        case 'O'://  foul tip on bunt
        case 'R'://  foul ball on pitchout
        case 'T'://  foul tip
        case 'Q'://  swinging on pitchout
            {
                System::Byte strikes = (System::Byte) (start>>0 & 0x03);
                endofAB = (strikes == 2) && (pitches[i] != 'F');
                if (!endofAB)
                {
                    strikes++;
                    start &= 0x3FCu;
                    start |= (System::Int16) (strikes << 0);
                    retval->Add(start);
                }
            }
            break;
        case 'H'://  hit batter
        case 'N'://  no pitch (on balks and interference calls)
        case 'X'://  ball put into play by batter
        case 'Y'://  ball put into play on pitchout
            endofAB = true;
            break;
        case 'U'://  unknown or missed pitch
        default:
            System::Diagnostics::Debug::Assert (false);
            break;
        }
    }
    System::Diagnostics::Debug::Assert (i==pitches->Length);
    while (eventidx < events->Length)
    {
        eventidx += ParseEvent (events->Substring(eventidx), &start);
    }
    if (endofAB)
    {
        System::Diagnostics::Debug::Assert ((start & 0xF) == 0);
        //start &= 0x3F;
    }
    retval->Add(start);
    return retval;
}

#if 0
System::Int32 ParseBasicEvent(System::String^ evt, System::Int16* state);
System::Int32 ProcessEvent(System::String^ events, System::Int32 *state)
{
    //https://www.retrosheet.org/eventfile.htm#3
    // A/B0/B1/Bn.n{-X}m;n{-X}m
    // A: basic play
    // B: fielders
    // nm: runner events
    // $ = [0-9]
    // $     A single fielder
    // $$    two or more fielders
    // FO(x) Force out @ x
    // SH    Sacrifice hit
    System::Byte part = 0;
    System::idx = 0;
    while (idx < events->Length)
    {
        switch (part)
        {
        case 0: idx += ParseBasicEvent(events->Substring(idx); break;
        case 1: break;
        case 2: break;
        }
    }
    System::Diagnostics::Debug::Assert (idx == events->Length);
    return 0;
}

System::Int32 ParseBasicEvent(System::String^ evt, System::Int16* state)
{
    System::idx = 0;
    while (idx < events->Length)
    {
        switch(evt[idx])
        {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
            while (evt[idx+1] >= '1' &&
                   evt[idx+1] <= '9')
               idx++;
            if (evt[idx+1] == '(')
            {
                idx+=2;
                System::Byte b = System::Convert::ToByte(evt[idx])
            }
            break;
        }
    }
}

System::Collections::Generic::List<System::Int16>^ GenSequence2 (System::Int16 start,
                                                                 System::String^ pitches,
                                                                 System::String^ events)
{
    System::Int32 idx = 0;
    while ((idx += ProcessEvent(events->Substring(idx))) < events->Length);
    System::Collections::Generic::List<System::Int16>^ retval =
        gcnew System::Collections::Generic::List<System::Int16>();
    // must start with a 0-0 count
    System::Diagnostics::Debug::Assert ((start & 0x0F) == 0);
    //retval->Add(start);
    System::Boolean endofAB = false;
    System::Int32 eventidx = 0;
    System::Int32 i;
    for (i=0; i<pitches->Length && !endofAB; i++)
    {
        switch (pitches[i])
        {
        case '*'://  indicates the following pitch was blocked by the catcher
        case '1'://  pickoff throw to first
        case '2'://  pickoff throw to second
        case '3'://  pickoff throw to third
        case '+'://  following pickoff throw by the catcher
            // no real change
            break;
        case '>'://  Indicates a runner going on the pitch
            eventidx += ParseEvent(events->Substring(eventidx), &start);
            break;
        case '.'://  marker for play not involving the batter
            break;
        case 'B'://  ball
        case 'I'://  intentional ball
        case 'M'://  missed bunt attempt
        case 'V'://  called ball because pitcher went to his mouth
        case 'P'://  pitchout
            {
                System::Byte balls = (System::Byte) (start>>2 & 0x03);
                endofAB = balls == 3;
                if (!endofAB)
                {   balls++;
                    start &= 0x3F3u;
                    start |= (System::Int16) (balls << 2);
                    retval->Add(start);
                }
            }
            break;
        case 'C'://  called strike
        case 'K'://  strike (unknown type)
        case 'S'://  swinging strike
        case 'F'://  foul
        case 'L'://  foul bunt
        case 'O'://  foul tip on bunt
        case 'R'://  foul ball on pitchout
        case 'T'://  foul tip
        case 'Q'://  swinging on pitchout
            {
                System::Byte strikes = (System::Byte) (start>>0 & 0x03);
                endofAB = (strikes == 2) && (pitches[i] != 'F');
                if (!endofAB)
                {
                    strikes++;
                    start &= 0x3FCu;
                    start |= (System::Int16) (strikes << 0);
                    retval->Add(start);
                }
            }
            break;
        case 'H'://  hit batter
        case 'N'://  no pitch (on balks and interference calls)
        case 'X'://  ball put into play by batter
        case 'Y'://  ball put into play on pitchout
            endofAB = true;
            break;
        case 'U'://  unknown or missed pitch
        default:
            System::Diagnostics::Debug::Assert (false);
            break;
        }
    }
    System::Diagnostics::Debug::Assert (i==pitches->Length);
    while (eventidx < events->Length)
    {
        eventidx += ParseEvent (events->Substring(eventidx), &start);
    }
    if (endofAB)
    {
        System::Diagnostics::Debug::Assert ((start & 0xF) == 0);
        //start &= 0x3F;
    }
    retval->Add(start);
    return retval;
}

#endif
System::Int16 ToInt16 (System::Byte Outs,
                       System::Boolean Third,
                       System::Boolean Second,
                       System::Boolean First,
                       System::Boolean Home,
                       System::Byte Balls,
                       System::Byte Strikes)
{
    // OO321HBBSS
    System::Diagnostics::Debug::Assert (Outs<4);
    System::Diagnostics::Debug::Assert (Balls<4);
    System::Diagnostics::Debug::Assert (Strikes<4);
    return ((System::Int16) (Outs<<8))         |
                            (Third?0x80:0x00)  |
                            (Second?0x40:0x00) |
                            (First?0x20:0x00)  |
                            (Home?0x10:0x00)   |
           ((System::Int16) (Balls<<2))        |
           ((System::Int16) (Strikes));
}


#define regTrans(a,b) System::Diagnostics::Debug::WriteLine((a).ToString("X3") + \
                                                            " -> " + \
                                                            (b).ToString("X3"))
System::Boolean ParseGame (System::IO::TextReader^ rdr,
                           array<System::UInt32, 2>^ trans)
{
#if 0
    System::Byte retInn = 0;
    while ((retInn = ParseInning (rdr, trans)) == 0);
    // 0 = (
    // 1 = next game
    // 2 = EOF
    return retInn == 2;
#else
    System::Boolean retval = false;
    System::Int16 state = ToInt16 (0, false, false, false, false, 0, 0);
    System::String^ line = rdr->ReadLine();
    System::Byte HomeAway = 0xFF;
    System::String^ LastAB = "";
    do
    {
        while ((line != nullptr)             &&
               (line->IndexOf("play,") != 0) &&
               (line->IndexOf("id,") != 0))
        {
            line = rdr->ReadLine();
        }
        if (line == nullptr)
        {
            retval = true;
            break;
        }
        if (line->IndexOf("id,") == 0)
        {
            break;
        }
        System::Diagnostics::Debug::Assert(line->IndexOf("play,") == 0);
        // play,7,1,gardb001,12,.BCC>B,SB2
        array<System::String^>^ split = line->Split(',');
        System::Diagnostics::Debug::Assert(split->Length == 7);
        System::Diagnostics::Debug::WriteLine (line);
        if (HomeAway != System::Convert::ToByte(split[2]))
        {
            HomeAway = System::Convert::ToByte(split[2]);
            // end of half inning
            state = ToInt16 (0, false, false, false, false, 0, 0);
        }
        System::Diagnostics::Debug::Assert (((LastAB == split[3]) && ((state & 0x10) != 0)) ||
                                            ((LastAB != split[3]) && ((state & 0x1F) == 0)));
        LastAB = split[3];
        if ((state & 0x010) == 0)
        {
            regTrans(state,(state | 0x010));
            trans[state,(state | 0x010)]++;
            state |= 0x010u;
        }
        else
        {
            // continuing at bat
            state *= 1;
        }
        System::Collections::Generic::List<System::Int16>^ seq =
            GenSequence(state, split[5], split[6]);
        for each (System::Int16 n in seq)
        {
            regTrans(state,n);
            trans[state,n]++;
            state = n;
        }
        /* remove count and runner at home */
        //regTrans(state,(state & 0x3E0));
        //trans[state,(state & 0x3E0)]++;
        //state &= 0x3E0;
        line = rdr->ReadLine();
    } while (true);
    return retval;
#endif
}

#if 0
System::Byte ParseInning (System::IO::TextReader^ rdr,
                          array<System::UInt32, 2>^ trans)
{
    System::Byte retval = 0;
    System::Int16 state = ToInt16 (0, false, false, false, false, 0, 0);
    System::String^ line = rdr->ReadLine();
    System::Byte HomeAway = 0xFF;
    do
    {
        while ((line != nullptr)             &&
               (line->IndexOf("play,") != 0) &&
               (line->IndexOf("id,") != 0))
        {
            line = rdr->ReadLine();
        }
        if (line == nullptr)
        {
            retval = 2;
            break;
        }
        if (line->IndexOf("id,") == 0)
        {
            retval = 1;
            break;
        }
        System::Diagnostics::Debug::Assert(line->IndexOf("play,") == 0);
        array<System::String^>^ split = line->Split(',');
        System::Diagnostics::Debug::Assert(split->Length == 7);
        System::Diagnostics::Debug::WriteLine (line);
        if (HomeAway != System::Convert::ToByte(split[2]))
        {
            HomeAway = System::Convert::ToByte(split[2]);
            // end of half inning
            state = ToInt16 (0, false, false, false, false, 0, 0);
        }
        System::Diagnostics::Debug::Assert((state & 0x1F) == 0);
        regTrans(state,(state | 0x010));
        trans[state,(state | 0x010)]++;
        state |= 0x010u;
        System::Collections::Generic::List<System::Int16>^ seq = GenSequence(state, split[5], split[6]);
        for each (System::Int16 n in seq)
        {
            regTrans(state,n);
            trans[state,n]++;
            state = n;
        }
        /* remove count and runner at home */
        //regTrans(state,(state & 0x3E0));
        //trans[state,(state & 0x3E0)]++;
        //state &= 0x3E0;
        line = rdr->ReadLine();
    } while (true);
    return retval;
}
#endif
System::Int32 ParseEvent (System::String^ evt, System::Int16* start)
{
    //System::Byte fielderNum = 0;
    System::Int32 idx = 0;
    System::Boolean done = false;
    while (!done)
    {
        switch (evt[idx])
        {
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            //fielderNum = System::Convert::ToByte(evt[idx]);
            while (evt[idx+1] >= '1' &&
                   evt[idx+1] <= '9')
                   idx++;
            done = true;
            break;
        case '(':
            {
                idx++; // get to #
                System::Byte baseNum = evt[idx]=='B'?0:
                                                     System::Convert::ToByte(evt[idx]);
                System::Diagnostics::Debug::Assert((*start & (1<<(4+baseNum))) != 0);
                *start &= ~(1<<(4+baseNum));
                idx++; // get to ')'
                System::Diagnostics::Debug::Assert(evt[idx] == ')');
            }
            done = true;
            break;
        case ')': // should have been munched
            System::Diagnostics::Debug::Assert (false);
            break;
        case 'S': // Single
            *start &= 0x3E0;
            *start |= 0x020;
            break;
        case 'D': // Double
            *start &= 0x3C0;
            *start |= 0x040;
            break;
        case 'T': // Triple
            *start &= 0x380;
            *start |= 0x080;
            break;
        case 'K': // strikeout
            {
                System::Byte outs = (*start >> 8);
                outs++;
                *start &= 0x0E0;
                *start |= ((System::Int16) outs<<8);
            }
            done = true;
            break;
        case '/': // modifier - ignore until end of string or "."
            while ((idx+1 < evt->Length) &&
                   (evt[idx+1] != '.'))
                idx++;
            //if (idx+1 == evt->Length)
            //{
            //    System::Byte Out = *start >> 8;
            //    Out++;
            //    *start &= 0x0E0;
            //    *start |= (Out << 8);
            //}
            done = true;
            break;
        case '.':
            System::Diagnostics::Debug::Assert (false);
            break;
        default:  // unhandled
            System::Diagnostics::Debug::Assert (false);
            break;
        }
        idx++;
    }
    return idx;
}


#if 0
System::Int32 ParseEvent2 (System::String^ evt, System::Int16* start)
{
}
#endif