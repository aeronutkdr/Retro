#include "pch.h"
#include "..\RetroCmd\MathUtils.h"
//using namespace System;

System::String^ Dump(array<System::Int32>^ a)
{
    System::String^ retval = L"arr Count = " + a->Length + System::Environment::NewLine;
    for (System::Int32 i = 0; i<a->Length; i++)
    {
        retval += L"[" + i + L"] = " + a[i] + System::Environment::NewLine;
    }
    return retval;
}
System::String^ Dump(array<System::Double, 2>^ a, array<System::Double>^ b)
{
    System::String^ retval = L"";
    System::Diagnostics::Debug::Assert(a->GetLength(0) == b->Length);
    for (System::Int32 i = 0; i < a->GetLength(0); i++)
    {
        retval += L"[" + i + L"] = {";
        for (System::Int32 j = 0; j < a->GetLength(1); j++)
        {
            retval += a[i, j] + ",";
        }
        retval += L"} = " + b[i] + System::Environment::NewLine;
    }
    return retval;
}
System::String^ Dump(array<System::Double>^ b)
{
    System::String^ retval = L"";
    for (System::Int32 i = 0; i < b->Length; i++)
    {
        retval += L"[" + i + L"] = " + b[i] + System::Environment::NewLine;
    }
    return retval;
}
#define ToVal(c) (((c)>'9')?(10+(c)-'A'):((c)-'0'))
array<System::Int32>^ BuildRLU(System::String^ flt)
{
    System::Int32 last = ToVal(flt[flt->Length - 1]);
    System::Int32 sub = 0;
    for (System::Int32 i = 0; (i < 4) && ((last & 1) == 0); sub++, i++, last >>= 1);
    System::Int32 num = flt->Length * 4 - sub;
    array<System::Int32>^ retval = gcnew array<System::Int32>(num);
    for (System::Int32 i = 0; i < num; i++) retval[i] = -1;
    System::Int32 count = 0;
    for (System::Int32 i = 0; i < flt->Length; i++)
    {
        System::Int32 v = ToVal(flt[i]);
        if (v & 0x8) { retval[4 * i + 0] = count; count++; }
        if (v & 0x4) { retval[4 * i + 1] = count; count++; }
        if (v & 0x2) { retval[4 * i + 2] = count; count++; }
        if (v & 0x1) { retval[4 * i + 3] = count; count++; }
    }
    return retval;
}
System::Void ProcessFile(System::String^ str,
                         array<System::Int32, 2>^ freq,
                         array<System::Int32>^ runs,
                         array<System::Int32>^ RLU2);
System::Int32 Process(array<System::Int32, 2>^ freq,
                      array<System::Int32>^ runs,
                      array<System::Int32>^ LU,
                      System::String^ str,
                      System::Int32 LastState);
#define START_OUTS (4)
//#define START_HOME (12)
#define START_1B (26)
#define START_2B (27)
#define START_3B (28)
#define BATTER_EV (35)
#define OUTS_ON_PLAY (40)
#define DEST_HOME (58)
#define DEST_1B (59)
#define DEST_2B (60)
#define DEST_3B (61)
/* 0         1         2         3         4         5         6         7         8         9      */
/* 0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456*/
/* 0000100000000000000000000011100000000000100000000000000000111100000000000000000000000000000000000*/
/* 08      00      00      38      00      80      00      3C      00      00      00      00      0*/
/* 080000380080003C000000000*/
/* 1234567890123456789012345*/
/* 080000380080003C         */
System::Void BuildSystem(array<System::Int32, 2>^ f,
    array<System::Int32>^ r,
    array<System::Double, 2>^ df,
    array<System::Double>^ dr);
//..\RetroEventMaestro\2021eve 2021TOR.EVA.txt FFFFFFFFFFFFFFFFFFFFFFFF8
//..\RetroEventMaestro\2021eve 2021*.EV?.txt FFFFFFFFFFFFFFFFFFFFFFFF8
// 3-7,26-29,35,40,58-61,78
// 00011111 00000000 00000000 00111100 00010000 10000000 00000000 00111100 00000000 00000010 00000000 00000000 0
// 1F 00 00 3C 10 80 00 3C 00 02 00 00 0
//..\RetroEventMaestro\2018eve 2018*.out 1F00003C1080003C000200000
int main(array<System::String^>^ args)
{
    for each (System::String ^ s in args)
    {
        System::Diagnostics::Debug::Write(s + L" ");
    }
    System::Diagnostics::Debug::WriteLineIf(args->Length > 0, L"");
    array<System::Int32>^ RLU2 = BuildRLU(args[2]);
    System::Diagnostics::Debug::Assert((RLU2->Length > DEST_3B) &&
        (RLU2[START_OUTS] != -1) &&
        //(RLU2[START_HOME]   != -1) &&
        (RLU2[START_1B] != -1) &&
        (RLU2[START_2B] != -1) &&
        (RLU2[START_3B] != -1) &&
        (RLU2[BATTER_EV] != -1) &&
        (RLU2[OUTS_ON_PLAY] != -1) &&
        (RLU2[DEST_HOME] != -1) &&
        (RLU2[DEST_1B] != -1) &&
        (RLU2[DEST_2B] != -1) &&
        (RLU2[DEST_3B] != -1));
    array<System::Int32, 2>^ freq = gcnew array<System::Int32, 2>(64, 64);
    array<System::Int32>^ runs = gcnew array<System::Int32>(64);
    freq->Initialize();
    runs->Initialize();
    array<System::String^>^ names = System::IO::Directory::GetFiles(args[0], args[1]);
    for each (System::String ^ s in names)
    {
        ProcessFile(s, freq, runs, RLU2);
    }
    //System::Diagnostics::Debug::Write(Dump(freq));
    array<System::Double, 2>^ dfreq = gcnew array<System::Double, 2>(freq->GetLength(0), freq->GetLength(1));
    array<System::Double>^ dsum = gcnew array<System::Double>(freq->GetLength(0));
    BuildSystem(freq, runs, dfreq, dsum);
    System::Diagnostics::Debug::Write(Dump(dfreq, dsum));
    GJ_Solve(dfreq, dsum);
    System::Diagnostics::Debug::Write(Dump(dsum));
    return 0;
}
System::Int32 Process(array<System::Int32, 2>^ freq,
                      array<System::Int32>^ runs,
                      array<System::Int32>^ LU,
                      System::String^ str,
                      System::Int32 LastState)
{
    array<System::String^>^ arr = str->Split(',');
    System::Int32 bases_start = ((arr[LU[START_3B]]->Trim('"')->Length) ? 0x8 : 0x00) |
                                ((arr[LU[START_2B]]->Trim('"')->Length) ? 0x4 : 0x00) |
                                ((arr[LU[START_1B]]->Trim('"')->Length) ? 0x2 : 0x00) | 1;
    System::Int32 outs_start = System::Convert::ToInt32 (arr[LU[START_OUTS]]->Trim('"'));
    System::Int32 outs_end = outs_start + System::Convert::ToInt32(arr[LU[OUTS_ON_PLAY]]->Trim('"'));
    System::Int32 DestH = System::Convert::ToInt32(arr[LU[DEST_HOME]]->Trim('"'));
    System::Int32 Dest1 = System::Convert::ToInt32(arr[LU[DEST_1B]]->Trim('"'));
    System::Int32 Dest2 = System::Convert::ToInt32(arr[LU[DEST_2B]]->Trim('"'));
    System::Int32 Dest3 = System::Convert::ToInt32(arr[LU[DEST_3B]]->Trim('"'));

    System::Boolean EndH = (arr[LU[BATTER_EV]]->Trim('"') == "F");
    System::Boolean End1B = (DestH == 1) || (Dest1 == 1);
    System::Boolean End2B = (DestH == 2) || (Dest1 == 2) || (Dest2 == 2);
    System::Boolean End3B = (DestH == 3) || (Dest1 == 3) || (Dest2 == 3) || (Dest3 == 3);
    System::Int32 bases_end = (End3B ? 0x8 : 0x00) |
                              (End2B ? 0x4 : 0x00) |
                              (End1B ? 0x2 : 0x00) |
                              (EndH  ? 0x1 : 0x00);

    System::Int32 start = (outs_start << 4) | bases_start;
    System::Int32 end   = (outs_end   << 4) | bases_end;
    runs[start] += ((DestH > 3) ? 1 : 0) +
                   ((Dest1 > 3) ? 1 : 0) +
                   ((Dest2 > 3) ? 1 : 0) +
                   ((Dest3 > 3) ? 1 : 0);
    System::Diagnostics::Debug::Assert(start < freq->GetLength(0));
    System::Diagnostics::Debug::Assert(end   < freq->GetLength(1));
    if ((LastState & 0x1) == 0)
    {
        /* to deal with extra inning / runner on 2nd */
        freq[(start & ~0x01), start]++;
    }
    freq[start, end]++;
    return end;
}
System::Void BuildSystem(array<System::Int32, 2>^ f,
                         array<System::Int32>^ r,
                         array<System::Double, 2>^ df,
                         array<System::Double>^ dr)
{
    System::Diagnostics::Debug::Assert(f->GetLength(0) == r->GetLength(0));
    for (System::Int32 i = 0; i < df->GetLength(0); i++)
    {
        System::Int32 isum = 0;
        dr[i] = -((System::Double)r[i]);
        for (System::Int32 j = 0; j < df->GetLength(1); j++)
        {
            isum += f[i, j];
            df[i, j] = (System::Double)f[i, j];
        }
        df[i, i] -= (System::Double)isum;
        if (isum == 0) df[i, i] = -1.;
    }
}
System::Void ProcessFile(System::String^ str,
    array<System::Int32, 2>^ freq,
    array<System::Int32>^ runs,
    array<System::Int32>^ RLU2)
{
    System::IO::TextReader^ rdr = gcnew System::IO::StreamReader(str);
    /* http://regexstorm.net/tester */
    //System::Text::RegularExpressions::Regex^ regex =
    //    gcnew System::Text::RegularExpressions::Regex(L"(?:\"?([^\",$]*)\"?[,$])*");
    /* optional " followed by
       array not including any of {",$} followed by
       optional " followed by
       one of {,$} */
    System::Int32 LastState = 0;
    for (;;)
    {
        System::String^ line = rdr->ReadLine();
        if (line == nullptr) break;
        LastState = Process(freq, runs, RLU2, line, LastState);
        if ((LastState >> 4) == 3)
        {
            LastState = 0;
        }
    }
    rdr->Close();
}