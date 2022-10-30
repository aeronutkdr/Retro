// RetroInningMaestro.cpp : main project file.
#include "stdafx.h"
#include "GameState.h"
using namespace System;

void ProcessTeam (System::String^ team, array<System::Int32, 2>^ freq,
                  System::Collections::Generic::Dictionary<System::Int32, System::Int32>^ dict);
System::Int32 PlayerVal(System::Int32 v);

System::Int32 Runs[1024] = {0};
// Debug/RetroInningMaestro 2018eve\evs.txt > evs.out
int main(array<System::String ^> ^args)
{
    System::Collections::Generic::Dictionary<System::Int32, System::Int32>^ FreqDict =
        gcnew System::Collections::Generic::Dictionary<System::Int32, System::Int32>;
    array<System::Int32, 2>^ FreqTable;
    FreqTable = gcnew array<System::Int32, 2>(1024,1024);
    FreqTable->Initialize();
    System::IO::TextReader^ rdr = gcnew System::IO::StreamReader(args[0]);
    System::String^ line;
    while ((line = rdr->ReadLine()) != nullptr)
    {
        ProcessTeam(line, FreqTable, FreqDict);
    }
    rdr->Close();
#if 1
    System::Int32 sum[1024] = {0};
    for each (System::Collections::Generic::KeyValuePair<System::Int32, System::Int32> kvp in FreqDict)
    {
        System::Int16 i = (kvp.Key>>16);
        System::Int16 j = (kvp.Key&0xFFFF);
        System::Diagnostics::Debug::Assert (i!=j);
#define MAX(a,b) ((a)>(b)?(a):(b))
        System::Int32 runValue = MAX(PlayerVal(i>>4) - PlayerVal(j>>4),0);
        Runs[i] -= runValue * kvp.Value;
        sum[i] -= kvp.Value;
        System::Console::WriteLine (i+","+
                                    j+","+
                                    kvp.Value);
    }
    for (System::Int32 i=0; i<1024; i++)
    {
        if (sum[i] != 0)
        {
            FreqDict->Add(((i<<16) | i), sum[i]);
            System::Console::WriteLine(i + "," + i + "," + FreqTable[i,i]);
        }
    }
#else
    for (System::Int32 i=0; i<1024; i++)
    {
        System::Diagnostics::Debug::Assert (FreqTable[i,i]==0);
        System::Int32 sum = 0;
        for (System::Int32 j=0; j<1024; j++)
        {
            System::Diagnostics::Debug::Assert ((FreqTable[i,j] == 0 && !FreqDict->ContainsKey(i<<16 | j)) ||
                                                (FreqTable[i,j] == FreqDict[i<<16 | j]));
            if (FreqTable[i,j] > 0)
            {
#define MAX(a,b) ((a)>(b)?(a):(b))
                System::Int32 runValue = MAX(PlayerVal(i>>4) - PlayerVal(j>>4),0);
                Runs[i] -= runValue * FreqTable[i,j];
                sum -= FreqTable[i,j];
                System::Console::WriteLine (i+","+
                                            j+","+
                                            FreqTable[i,j]);
            }
        }
        System::Diagnostics::Debug::Assert(FreqTable[i,i] == 0);
        if (sum != 0)
        {
            FreqTable[i,i] = sum;
            System::Console::WriteLine(i + "," + i + "," + FreqTable[i,i]);
        }
    }
#endif
    for (System::Int32 i=0; i<1024; i++)
    {
        System::Console::WriteLine(i + "," + Runs[i] + ",0");
    }
    return 0;
}

void ProcessTeam (System::String^ team, array<System::Int32, 2>^ freq,
                  System::Collections::Generic::Dictionary<System::Int32, System::Int32>^ dict)
{
    System::Diagnostics::Debug::Assert(freq->GetLength(0) == 1024);
    System::Diagnostics::Debug::Assert(freq->GetLength(1) == 1024);
    System::IO::TextReader^ rdr = gcnew System::IO::StreamReader(team);
    System::String^ line;
    System::Int16 StateVal = -1;
    //System::Int32 count = 0;
    GameState state;
    while ((line = rdr->ReadLine()) != nullptr)
    {
        //count++;
        state.ProcessEvent(line->Split(','), freq, dict);
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