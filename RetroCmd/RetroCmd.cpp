// RetroCmd.cpp : main project file.
#include "stdafx.h"
#include "MathUtils.h"
#include "Contour.h"

using namespace System;

System::Byte Outs (System::Byte b)
{
    return b>>4;
}

System::Byte Runners (System::Byte b)
{
    System::Byte retval = 0;
    retval += b&0x8?1:0;
    retval += b&0x4?1:0;
    retval += b&0x2?1:0;
    retval += b&0x1?1:0;
    return retval;
}

System::Byte Runs (System::Byte from, System::Byte to)
{
    System::Byte Outs_to = Outs   (to  );
    System::Byte numOuts    = Outs_to - Outs   (from);
    System::Byte numRunners = Runners(from) - Runners(to  );
    // if this is a transition where a player comes to the plate
    if (((from & 1) == 0) &&
        ((to   & 1) == 1))
        numRunners++;
    System::Diagnostics::Trace::Assert (numRunners >= numOuts);
    System::Byte retRuns = numRunners-numOuts;
    return retRuns;
}

System::Void Dump (array<System::Double, 2>^ a)
{
    for (System::Int32 i=0; i<a->GetLength(0); i++)
    {
        for (System::Int32 j=0; j<a->GetLength(1); j++)
        {
            System::Console::Write (a[i,j] + "\t");
        }
        System::Console::WriteLine();
    }
}
System::Void Dump (array<System::Double, 1>^ r)
{
    for (System::Int32 j=0; j<r->GetLength(0); j++)
    {
        System::Console::WriteLine ("x[" + j.ToString("D2") + "]\t" +
                                    r[j].ToString("F3"));
    }
}
System::Void TestGCF(System::Int32 a, System::Int32 b)
{
    System::Console::WriteLine ("(" + a + ", " + b + ") = " + GCF (a,b));
}

int main(array<System::String ^> ^args)
{
    //goto gcf;
    //goto contour;
    System::Collections::Generic::Dictionary<System::String^, array<System::Byte>^>^ Scores =
        gcnew System::Collections::Generic::Dictionary<System::String^, array<System::Byte>^>();
    Retro::StringList^ PlayerList = gcnew Retro::StringList("players.txt");
    Retro::StringList^ TeamList   = gcnew Retro::StringList("teams.txt");
    Retro::StringList^ GameList   = gcnew Retro::StringList("games.txt");

    System::IO::DirectoryInfo^ EvtDir = gcnew System::IO::DirectoryInfo
                              ("C:\\Users\\Kevin\\My Documents\\retrosheet\\data\\bevent");
                              //("F:\\_Personal\\baseball\\retrosheet\\2010eve");
    System::IO::DirectoryInfo^ ScoDir = gcnew System::IO::DirectoryInfo
                              ("C:\\Users\\Kevin\\My Documents\\retrosheet\\data\\bgame");
    //array<System::String^>^ srch_strs = {"1999*.txt", "2000*.txt", "2001*.txt", "2002*.txt"};
    array<System::String^>^ srch_strs = {"2011WAS.txt"};
    //array<System::String^>^ srch_strs = {"2011WAS_All.txt"};
    //array<System::String^>^ srch_strs = {"2011*.txt"};
    //array<System::Int32,2>^ freq = gcnew array<System::Int32,2>(64,64);
    array<System::Double,2>^ freq = gcnew array<System::Double,2>(64,64);
    freq->Initialize();
    for each (System::String^ str in srch_strs)
    {
        array<System::IO::FileInfo^>^ Files = ScoDir->GetFiles(str);
        for each (System::IO::FileInfo^ file in Files)
        {
            System::Console::WriteLine ("Reading: " + file->Name);
            System::IO::TextReader^ rdr = gcnew System::IO::StreamReader
                                                                  (file->FullName);
            System::String^ lin;
            while ( (lin=rdr->ReadLine()) != nullptr)
            {
                array<System::Char>^splitChars = {','};
                array<System::Char>^trimChars = {'\"'};
                array<System::String^>^split = lin->Split(splitChars);
                System::Diagnostics::Trace::Assert (split->Length == 3);
                Scores->Add (split[0]->Trim(trimChars),
                             gcnew array<System::Byte> {System::Convert::ToByte (split[1]),
                                                        System::Convert::ToByte (split[2])});
            }
        }
    }
    array <System::Byte>^ MyScores = gcnew array<System::Byte>(2);
    for each (System::String^ str in srch_strs)
    {
        array<System::IO::FileInfo^>^ Files = EvtDir->GetFiles(str);
        //array<System::IO::FileInfo^>^ Files = EvtDir->GetFiles("all.txt");
        for each (System::IO::FileInfo^ file in Files)
        {
            System::Console::WriteLine ("Reading: " + file->Name);
            System::IO::TextReader^ rdr   = gcnew System::IO::StreamReader
                                                                  (file->FullName);
            System::String^ evt;
            System::Int32 LastGame = -1;
            System::String^ LastGameName;
            System::Int32 NumRuns = 0;
            while ( (evt=rdr->ReadLine()) != nullptr)
            {
                //System::Console::WriteLine(evt);
                Retro::Event^ e = gcnew Retro::Event(evt,
                                                     PlayerList,
                                                     GameList,
                                                     TeamList,
                                                     false);
                System::Byte from = e->From();
                System::Byte to   = e->To();
                System::Console::WriteLine (e->Dump());
                freq[from,to]++;
                if (e->RunnersLost() > 0)
                {
                    System::Console::WriteLine (e->RunnersLost() + "\t" + evt);
                }
                /**/
                if (LastGame != e->GameID())
                {
                    if (false && LastGame != -1)
                    {
                        array<System::Byte>^ tmp = Scores[LastGameName];
                        if (MyScores[0] != tmp[0] ||
                            MyScores[1] != tmp[1])
                        {
                            System::Console::WriteLine (LastGameName + "\t" +
                                                        tmp[0] + "\t" +
                                                        tmp[1] + "\t" +
                                                        MyScores[0] + "\t" +
                                                        MyScores[1]);
                        }
                    }
                    MyScores[0] = MyScores[1] = 0;
                    //System::Console::WriteLine ("Game #" + LastGame + "\t" + NumRuns);
                    LastGameName = e->m_GameName;
                    LastGame = e->GameID();
                }
                /**/
                NumRuns += Runs (from, to);
                MyScores[e->HomeTeamBatting()?1:0] += Runs (from, to);
                // add a transition from plate empty to plate filled
                if ((Outs(to) < 3) &&
                    (e->PlateEmpty()))
                {
                    freq[to,to|1]++;
                }
            }
            rdr->Close();
            for (System::Int32 i=0; i<freq->GetLength(0); i++)
            {
                for (System::Int32 j=0; j<freq->GetLength(1); j++)
                {
                    if (freq[i,j] > 0)
                    System::Diagnostics::Debug::Print (i.ToString() + "," + j.ToString() + "," + freq[i,j].ToString());
                }
            }
            if (false)
            {
                array<System::Byte>^ tmp = Scores[LastGameName];
                if (MyScores[0] != tmp[0] ||
                    MyScores[1] != tmp[1])
                {
                    System::Console::WriteLine (LastGameName + "\t" +
                                                tmp[0] + "\t" +
                                                tmp[1] + "\t" +
                                                MyScores[0] + "\t" +
                                                MyScores[1]);
                }
            }
            //System::Console::WriteLine ("Game #" + LastGame + " = " + NumRuns);
        }
    }
    PlayerList->Write("players.txt");
    TeamList->Write("teams.txt");
    GameList->Write("games.txt");
//    array<System::Int32, 1>^ b =
//                             gcnew array<System::Int32, 1>(freq->GetLength(0));
    array<System::Double, 1>^ b =
                             gcnew array<System::Double, 1>(freq->GetLength(0));
    b->Initialize();
    for (System::Int32 i=0; i<freq->GetLength(0); i++)
    {
        //System::Int32 sum = 0;
        System::Double sum = 0;
        for (System::Int32 j=0; j<freq->GetLength(1); j++)
        {
            sum += freq[i,j];
            if (freq[i,j]) b[i] -= freq[i,j] * Runs(i, j);
        }
        freq[i,i] -= sum;
    }
    for (System::Int32 i=48; i<freq->GetLength(0); i++)
    {
        for (System::Int32 j=0; j<freq->GetLength(0); j++)
        {
            if (i==j) continue;
            freq[i,i] -= freq[j,i];
        }
        /* make sure each pivot has at least one with no value */
        if (freq[i,i]==0)
        {
            System::Diagnostics::Trace::Assert (b[i]==0);
            freq[i,i]=1;
       }
    }
    Dump(freq);
    Dump(b);
#if 0
    Dump(freq);
    Dump(b);
    array<System::Double,2>^ freq_pos = gcnew array<System::Double,2>(8,8);
    array<System::Double,1>^ b_pos    = gcnew array<System::Double,1>(8);
    array<System::Double,2>^ freq_out = gcnew array<System::Double,2>(4,4);
    array<System::Double,1>^ b_out    = gcnew array<System::Double,1>(4);
    array<System::Double,2>^ freq_any = gcnew array<System::Double,2>(1,1);
    array<System::Double,1>^ b_any    = gcnew array<System::Double,1>(1);
    freq_pos->Initialize();
    freq_out->Initialize();
    freq_any->Initialize();
    b_pos->Initialize();
    b_out->Initialize();
    b_any->Initialize();
    for (System::Int32 i=0; i<freq->GetLength(0); i++)
    {
        System::String^ line = "";
        for (System::Int32 j=0; j<freq->GetLength(1); j++)
        {
            line += freq[i,j] + "\t";
            freq_pos[i &7,j &7] += freq[i,j];
            freq_out[i>>3,j>>3] += freq[i,j];
            freq_any[i &0,j &0] += freq[i,j];
        }
        b_pos[i &7] += b[i];
        b_out[i>>3] += b[i];
        b_any[i &0] += b[i];
        line += b[i];
        System::Diagnostics::Debug::Print (line);
    }
#endif
    System::Console::WriteLine ("Solve freq");
    //Dump(GJ_Solve (freq    , b    ));
    GJ_Solve (freq    , b    );
    Dump(freq);
    Dump(b);
    //System::Console::WriteLine ("Solve freq pos");
    //Dump(GJ_Solve (freq_pos, b_pos));
    //GJ_Solve (freq_pos, b_pos);
    //System::Console::WriteLine ("Solve freq out");
    //Dump(GJ_Solve (freq_out, b_out));
    //GJ_Solve (freq_out, b_out);
    //System::Console::WriteLine ("Solve all");
    //Dump(GJ_Solve (freq_any, b_any));
    //GJ_Solve (freq_any, b_any);
    goto end;
gcf:
    TestGCF (1609344, 1000000);
    TestGCF (1609345, 1000000);
    TestGCF (  25146,   15625);
    array<System::Int32, 1>^ arr1 = {16,66,67,73,41,85,32,27,68,76};
    array<System::Int32, 1>^ arr2 = {16,67,73,41,85,32,27,68,76};
    array<System::Int32, 1>^ arr3 = {16,73,41,85,32,27,68,76};
    array<System::Int32, 1>^ arr4 = {16,41,85,32,27,68,76};
    array<System::Int32, 1>^ arr5 = {16,85,32,27,68,76};
    array<System::Int32, 1>^ arr6 = {16,32,27,68,76};
    array<System::Int32, 1>^ arr7 = {16,27,68,76};
    array<System::Int32, 1>^ arr8 = {16,68,76};
    array<System::Int32, 1>^ arr9 = {16,76};
    array<System::Int32, 1>^ arrA = {1609344, 1000000};
    array<System::Int32, 1>^ arrB = {1000000, 1609344};
    System::Console::WriteLine (GCF(arr1).ToString());
    System::Console::WriteLine (GCF(arr2).ToString());
    System::Console::WriteLine (GCF(arr3).ToString());
    System::Console::WriteLine (GCF(arr4).ToString());
    System::Console::WriteLine (GCF(arr5).ToString());
    System::Console::WriteLine (GCF(arr6).ToString());
    System::Console::WriteLine (GCF(arr7).ToString());
    System::Console::WriteLine (GCF(arr8).ToString());
    System::Console::WriteLine (GCF(arr9).ToString());
    System::Console::WriteLine (GCF(arrA).ToString());
    System::Console::WriteLine (GCF(arrB).ToString());
    TestGCF(  25859147,  13535744);
    TestGCF(     46741,     62976);
    TestGCF(   7029633,    618688);
    TestGCF(  14738849,   7871360);
    TestGCF(    305869,     28736);
    TestGCF(    369451,    412864);
    TestGCF( 479068113, 127452416);
    TestGCF(1707690285,1693118912);
    TestGCF(   4185192,   7120603);
    TestGCF( 155912167,  92717481);
    TestGCF(     23071,     79725);
    TestGCF(1574832184,1081511493);
    TestGCF(1887677767,2050184849);
    TestGCF(1441382608, 450306105);
    TestGCF(  71145155, 521819393);
    TestGCF(1059234869, 322633719);
    TestGCF ( 1609344,  1000000);
    TestGCF ( 1609344, -1000000);
    TestGCF (-1609344,  1000000);
    TestGCF (-1609344, -1000000);
    TestGCF ( 1000000,  1609344);
    TestGCF ( 1000000, -1609344);
    TestGCF (-1000000,  1609344);
    TestGCF (-1000000, -1609344);
    TestGCF ( 16,  64);
    TestGCF ( 16, -64);
    TestGCF (-16,  64);
    TestGCF (-16, -64);
    TestGCF ( 64,  16);
    TestGCF ( 64, -16);
    TestGCF (-64,  16);
    TestGCF (-64, -16);
    array<Fraction, 2>^ a_arr =
                            {{Fraction( 2,1), Fraction( 1,1), Fraction(-1,1)},
                             {Fraction(-3,1), Fraction(-1,1), Fraction( 2,1)},
                             {Fraction(-2,1), Fraction( 1,1), Fraction( 2,1)}};
    array<Fraction, 1>^ b_arr =
                            {Fraction( 8,1), Fraction(-11,1), Fraction(-3,1)};
    //GJ_Solve (a_arr, b_arr);
    GJ_Solve<Fraction> (a_arr, b_arr);
    //NT_Solve (a_arr, b_arr);
    for (System::Int32 i=0; i<a_arr->GetLength(0); i++)
    {
        for (System::Int32 j=0; j<a_arr->GetLength(1); j++)
        {
            System::Console::Write (a_arr[i,j].Dump() + "  ");
        }
        System::Console::WriteLine (b_arr[i].Dump());
    }
    //Fraction a(1,2);
    //Fraction bb(1,3);
    //Fraction bbb = a * bb;
    //System::Console::WriteLine (bbb.Dump());
    goto end;
contour:
#if 1
    array <Double, 2>^ vals   = {{0.639, 0.356, 0.144, 0.000},
                                 {1.155, 0.744, 0.389, 0.000},
                                 {1.308, 0.820, 0.388, 0.000},
                                 {1.627, 1.028, 0.511, 0.000},
                                 {1.547, 1.078, 0.421, 0.000},
                                 {2.007, 1.347, 0.608, 0.000},
                                 {2.111, 1.471, 0.627, 0.000},
                                 {2.474, 1.627, 0.827, 0.000}};
    //array <Double, 2>^ vals   = {{0, 2, 1},
    //                             {1, 4, 3},
    //                             {0, 3, 2}};
    array <Double, 1>^ x_idxs   = {0, 1, 2, 3, 4, 5, 6, 7};
    array <Double, 1>^ y_idxs   = {0, 1, 2, 3};
    array <Double, 1>^ levels = {0, 0.5, 1, 1.5, 2, 2.5};
    System::Collections::Generic::List<array<System::Double, 1>^>^
        contour = conrec (vals, x_idxs, y_idxs, levels);
    reorder (contour);

    for each (array<System::Double, 1>^ a in contour)
    {
        System::Console::WriteLine
         (System::String::Format
           ("{0:F5}\t{1:F5}\t{2:F5}", a[0],a[1],a[4]));
        System::Console::WriteLine
         (System::String::Format
           ("{0:F5}\t{1:F5}\t{2:F5}", a[2],a[3],a[4]));
    }
    for each (array<System::Double, 1>^ a in contour)
    {
        System::Console::WriteLine
         (System::String::Format
           ("{0:F5}\t{1:F5}\t{2:F5}\t{3:F5}\t{4:F5}",
                                    a[0],a[1],a[2],a[3],a[4]));
    }
#else
    double vals[][3] = {{0, 1, 2},
                        {0, 1, 3},
                        {1, 2, 4}};
    double idxs[] = {0, 1, 2};
    double levels[] = {0.5, 1.5, 2.5, 3.5};
    conrec ((double*)vals, 0, 2, 0, 2, idxs, idxs, 4, levels);
#endif
end:
    return 0;
}