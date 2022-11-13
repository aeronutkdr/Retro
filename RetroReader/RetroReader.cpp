// RetroReader.cpp : main project file.
#include "stdafx.h"
#include <memory>
using namespace System;
System::Collections::Generic::List<System::Byte>^
              ProcessPitches(System::Int32   start,
                             System::String^ pitches);
#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))
/* 3 balls, 63 strikes, 4 outs, 4 bases   */
/* 2      +  6        + 2     + 4 =    14 */
/* 1<<14                          = 16384 */
/* OOBBBBCCCCCCCC */
#define BASE_BITS  (4)
#define COUNT_BITS (8)
#define OUT_BITS   (2)
#define NUM_BITS (COUNT_BITS + OUT_BITS + BASE_BITS)
#define TBL_SIZE (1<<NUM_BITS)
System::Int32 matrix[TBL_SIZE][TBL_SIZE];
#define Idx(O,B,C) (((O)<<12)+((B)<<8)+(C))
#define MASK Idx(((1<<OUT_BITS  )-1),\
                 ((1<<BASE_BITS )-1),\
                 ((1<<COUNT_BITS)-1))

System::Byte Value(System::Int32 x);
int main(array<System::String ^> ^args)
{
    memset (matrix, 0, sizeof(matrix));
    System::IO::TextReader^ rdr;
    rdr = gcnew System::IO::StreamReader(L"filters.txt");
    rdr->ReadLine();
    rdr->ReadLine();
    System::String^ flt = gcnew System::String("");
    System::String^ line;
    while ( (line = rdr->ReadLine()) != nullptr)
    {
        array<System::String^>^ split = line->Split('\t');
        flt += split[0] + ",";
    }
    rdr->Close();
    flt = flt->Substring(0,flt->Length-1);
    flt += "$";
    System::Diagnostics::Debug::WriteLine(flt);

    System::Text::RegularExpressions::Regex^ regex = gcnew System::Text::RegularExpressions::Regex(flt);
    array<System::String^>^ names =
       System::IO::Directory::GetFiles(
                                       //L"C:\\Users\\Kevin\\Documents\\retrosheet\\data\\2018eve",
                                       //L"2018*.ev?.txt");
                                       L"C:\\Users\\Kevin\\source\\repos\\Retro\\RetroEventMaestro\\2021eve",
                                       L"2021*.ev?.txt");
    System::Byte MaxStrikes = 0;
    for each (System::String^ s in names)
    {
        System::Diagnostics::Debug::WriteLine(s);
        rdr = gcnew System::IO::StreamReader(s);
        System::String^ PrevPitches = L"";
        System::Collections::Generic::List<System::Byte>^ counts =
                gcnew System::Collections::Generic::List<System::Byte>;
        System::Boolean batterEvent;
        System::Byte val = 0;
        System::Byte BasesStart;
        System::Byte BasesEnd;
        System::Byte OutsStart;
        System::Byte OutsEnd;
        while ((line = rdr->ReadLine()) != nullptr)
        {
            System::Text::RegularExpressions::Match^ m = regex->Match(line);
            System::Boolean EndGame = m->Groups[80]->Captures[0]->Value == "T";

            System::Boolean Home   = m->Groups[11]->Captures[0]->Value != "";
            System::Diagnostics::Debug::Assert (Home);
            System::Boolean First  = m->Groups[27]->Captures[0]->Value != "";
            System::Boolean Second = m->Groups[28]->Captures[0]->Value != "";
            System::Boolean Third  = m->Groups[29]->Captures[0]->Value != "";
            BasesStart = (Third ?0x08:0) |
                         (Second?0x04:0) |
                         (First ?0x02:0) |
                         (Home  ?0x01:0);
            OutsStart = System::Convert::ToInt32 (m->Groups[5]->Captures[0]->Value);
            batterEvent = m->Groups[36]->Captures[0]->Value == "T";
            counts->AddRange(ProcessPitches(val,
                                            m->Groups[8]->
                                                Captures[0]->
                                                    Value->
                                                        Substring(PrevPitches->Length)));
            if (counts->Count > 0)
            {
                val = counts[counts->Count-1];
            }
            else
            {
                val = 0;
            }
            System::Byte balls = val >> 6;
            System::Byte strikes = val & 0x3F;
            System::Diagnostics::Debug::WriteLineIf(balls > 3, balls + L" balls > 3: " + line);
            System::Diagnostics::Debug::WriteLineIf(balls != System::Convert::ToByte(m->Groups[6]->Captures[0]->Value),
                                                    balls + L" balls != expected: " + line); 
            System::Diagnostics::Debug::WriteLineIf(MIN(2,strikes) != System::Convert::ToByte(m->Groups[7]->Captures[0]->Value),
                                                    strikes + L" strikes != expected: " + line);
            Home = !batterEvent;
            First = m->Groups[59]->Captures[0]->Value == "1" ||
                    m->Groups[60]->Captures[0]->Value == "1";
            Second = m->Groups[59]->Captures[0]->Value == "2" ||
                     m->Groups[60]->Captures[0]->Value == "2" ||
                     m->Groups[61]->Captures[0]->Value == "2";
            Third = m->Groups[59]->Captures[0]->Value == "3" ||
                    m->Groups[60]->Captures[0]->Value == "3" ||
                    m->Groups[61]->Captures[0]->Value == "3" ||
                    m->Groups[62]->Captures[0]->Value == "3";
            BasesEnd = (Third ?0x08:0) |
                       (Second?0x04:0) |
                       (First ?0x02:0) |
                       (Home  ?0x01:0);
            OutsEnd = OutsStart + System::Convert::ToInt32 (m->Groups[41]->Captures[0]->Value);

            if (EndGame || (OutsEnd > 2) || batterEvent)
            {
                System::Diagnostics::Debug::Assert (OutsEnd <= 3);
                if (counts->Count > 0)
                {
                    val = counts[counts->Count-1];
                    System::Byte balls = val >> 6;
                    System::Byte strikes = val & 0x3F;
                    if (strikes > MaxStrikes)
                    {
                        System::Diagnostics::Debug::WriteLine(line);
                        MaxStrikes = strikes;
                        System::Diagnostics::Debug::Write("00");
                        for each (System::Byte b in counts)
                        {
                            System::Diagnostics::Debug::Write ("->" + (b>>6).ToString() +
                                                                      (b&0x3F).ToString());
                        }
                        System::Diagnostics::Debug::WriteLine("");
                    }
                    System::Byte b_prev = 0;
                    if ((BasesEnd & 1)==0)
                    {
                        if (Idx(OutsEnd,BasesEnd,0) == 833 &&
                            Idx(OutsEnd,BasesEnd+1,0) == 8448)
                        {
                            System::Console::WriteLine(__LINE__);
                        }
                        matrix[Idx(OutsEnd,BasesEnd,0)][Idx(OutsEnd,BasesEnd+1,0)]++;
                    }
                    for each (System::Byte b in counts)
                    {
                        if (Idx(OutsStart,BasesStart,b_prev) == 833 &&
                            Idx(OutsStart,BasesStart,b) == 8448)
                        {
                            System::Console::WriteLine(__LINE__);
                        }
                        matrix[Idx(OutsStart,BasesStart,b_prev)][Idx(OutsStart,BasesStart,b)]++;
                        b_prev = b;
                    }
                    if (Idx(OutsStart,BasesStart,b_prev) == 833 &&
                        Idx(OutsEnd,BasesEnd,0) == 8448)
                    {
                        System::Console::WriteLine(__LINE__);
                    }
                    matrix[Idx(OutsStart,BasesStart,b_prev)][Idx(OutsEnd,BasesEnd,0)]++;
                }
                else if (batterEvent)
                {
                    if (Idx(OutsStart,BasesStart,0) == 833 &&
                        Idx(OutsEnd,BasesEnd,0) == 8448)
                    {
                        System::Console::WriteLine(__LINE__);
                    }
                    matrix[Idx(OutsStart,BasesStart,0)][Idx(OutsEnd,BasesEnd,0)]++;
                }
                val = 0;
                counts->Clear();
                PrevPitches = L"";
            }
            else
            {
                if (counts->Count) val = counts[counts->Count-1];
                else val = 0;
                System::Diagnostics::Debug::Assert
                                (m->Groups[8]->Captures[0]->Value->IndexOf(PrevPitches) == 0);
                if (m->Groups[8]->Captures[0]->Value->Length > 0)
                    PrevPitches = m->Groups[8]->Captures[0]->Value->Substring(0, m->Groups[8]->Captures[0]->Value->Length-1);
                else
                    PrevPitches = "";
            }
        }
        rdr->Close();
    }
    System::Diagnostics::Debug::WriteLine ("MaxStrikes = " + MaxStrikes);
    for (System::Int32 i=0; i<TBL_SIZE; i++)
    {
        System::Int32 sum = 0;
        for (System::Int32 j=0; j<TBL_SIZE; j++)
        {
            sum += matrix[i][j];
        }
        matrix[i][i] -= sum;
    }
    System::Int32 SumRuns[TBL_SIZE];
    System::Collections::Generic::List<System::Int32>^ States =
        gcnew System::Collections::Generic::List<System::Int32>();
    for (System::Int32 i=0; i<TBL_SIZE; i++)
    {
        SumRuns[i] = 0;
        for (System::Int32 j=0; j<TBL_SIZE; j++)
        {
            System::Byte ToVal = Value(j);
            /* if adding a batter like 0x0000 -> 0x0100: */
            if (((j&i) == i) &&
                ((j^i) == 0x100) &&
                ((j&0xFF) == 0) &&
                ((i&0xFF) == 0))
            {
                ToVal--;
            }
            SumRuns[i] -= matrix[i][j] * ToVal;
            //System::Console::Write (matrix[i][j] + ",");
            if (matrix[i][j])
            {
                //System::Console::WriteLine (i.ToString("D") + "," +
                //                            j.ToString("D") + "," +
                //                            matrix[i][j]);
                if (!States->Contains(i)) States->Add(i);
                if (!States->Contains(j)) States->Add(j);
            }
        }
        //System::Console::WriteLine();
    }
    //for (System::Int32 i=0; i<TBL_SIZE; i++)
    //{
    //    System::Console::WriteLine (i.ToString("D") + "," +
    //                                SumRuns[i].ToString("D") + ",0");
    //}
    States->Sort();
    System::Console::WriteLine (States->Count + ",0,0");
    for each (System::Int32 s in States)
    {
        //System::Console::WriteLine(s);
        System::Console::WriteLine (s.ToString("D") + "," +
                                    SumRuns[s].ToString("D") + ",0");
    }
    for (System::Int32 i=0; i<TBL_SIZE; i++)
    {
        for (System::Int32 j=0; j<TBL_SIZE; j++)
        {
            if (matrix[i][j])
            {
                System::Console::WriteLine (States->IndexOf(i).ToString("D") + "," +
                                            States->IndexOf(j).ToString("D") + "," +
                                            matrix[i][j]);
            }
        }
    }
    return 0;
}

// BBSSSSSS (3 balls, 63 strikes)
System::Collections::Generic::List<System::Byte>^
              ProcessPitches(System::Int32   start,
                             System::String^ pitches)
{
    System::Collections::Generic::List<System::Byte>^ retval =
                    gcnew System::Collections::Generic::List<System::Byte>;
    System::Byte balls = start>>6;
    System::Byte strikes = start & 0x3F;
    for (System::Int32 i=0; i<pitches->Length-1; i++)
    {
        switch (pitches[i])
        {
        case 'B'://  ball
        case 'I'://  intentional ball
        case 'V'://  called ball because pitcher went to his mouth
        case 'P'://  pitchout
            // "CHA201007100","KCA",6,1,2,3,2,"CBFFBFBBFFX"
            // com,"$Batter received 4 balls but stayed at plate for 3 more pitches"
            if (balls < 3)
            {
                balls++;
            }
            break;

        case 'C'://  called strike
        case 'S'://  swinging strike
        case 'F'://  foul
        case 'L'://  foul bunt
        case 'O'://  foul tip on bunt
        case 'R'://  foul ball on pitchout
        case 'T'://  foul tip
        case 'Q'://  swinging on pitchout
        case 'M'://  missed bunt attempt
        {
            System::Boolean newstrike = true ||
                                        (strikes<2) ||
                                        ((pitches[i] != 'F') &&
                                         (pitches[i] != 'R'));
            System::Diagnostics::Debug::Assert (strikes < 63);
            if (newstrike)
            {
                strikes++;
            }
        }
        break;

        case '*'://  indicates the following pitch was blocked by the catcher
        case '1'://  pickoff throw to first
        case '2'://  pickoff throw to second
        case '3'://  pickoff throw to third
        case '7'://  pickoff throw to left field?
        case '+'://  following pickoff throw by the catcher
        case '>'://  Indicates a runner going on the pitch
        case '.'://  marker for play not involving the batter
        case 'H'://  hit batter
        case 'N'://  no pitch (on balks and interference calls)
        case 'X'://  ball put into play by batter
        case 'Y'://  ball put into play on pitchout
        case 'K'://  strike (unknown type)
        case 'D'://  not described by website but used in 2015ATL.EVN "BFBFDX"
        break;

        case 'U'://  unknown or missed pitch
        default:
            System::Diagnostics::Debug::Assert (false);
            break;
        }
        retval->Add((balls << 6) | strikes);
    }
    return retval;
}

/* OOBBBBCCCCCCCC */
/* 32109876543210 */
System::Byte Value(System::Int32 x)
{
    x>>=8;
    System::Byte retval = (x>>4);
    x &= 0x0F;
    while(x)
    {
        retval += (x&1);
        x >>= 1;
    }
    return retval;
}
