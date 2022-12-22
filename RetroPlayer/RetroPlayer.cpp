#include "pch.h"

using namespace System;

System::String^ DumpValues(System::Double v[][8]);
System::String^ DumpTransactions(System::Double t[][32]);
System::String^ DumpFreq(System::Int32 v[][32]);
System::String^ DumpMatch(System::Text::RegularExpressions::Match^ m);
System::Double CalcValue(System::Double v[][8], System::Int32 from, System::Int32 to);
/* "C:\Users\Kevin\source\repos\Retro\RetroEventMaestro\OO321 2021.txt" "C:\Users\Kevin\source\repos\Retro\RetroEventMaestro\2021eve" "2021*.ev?.txt" "judga001" */
/* "C:\Users\Kevin\source\repos\Retro\RetroEventMaestro\OO321 2021.txt" "C:\Users\Kevin\source\repos\Retro\RetroEventMaestro\2021eve" "2021*.ev?.txt" "[^,]*" */
ref struct DataType
{
    System::Int32 n;
    System::Double total;
    DataType(void) { n = 0; total = 0.; }
    System::String^ ToString(System::Void) override
    {
        return n + "\t" + total.ToString("F5") + "\t" + (total / (System::Double)n).ToString("F5");
    }
};
int main(array<System::String^>^ args)
{
    if (args->Length < 4)
    {
        System::Console::WriteLine("usage: " +
            System::AppDomain::CurrentDomain->FriendlyName +
            " <values path> <ev path> <ev filter> <name>");
        return -1;
    }
    System::Double Values[4][8];
    System::IO::TextReader^ rdr = (System::IO::TextReader^)gcnew System::IO::StreamReader(args[0]);
    (void)rdr->ReadLine();
    System::String^ line;
    for (System::Int32 i = 0; i < 4; i++)
    {
        line = rdr->ReadLine();
        array<System::String^>^ arr = line->Split(' ');
        for (System::Int32 j = 0; j < 8; j++)
        {
            Values[i][j] = System::Convert::ToDouble(arr[j + 1]);
        }
    }
    rdr->Close();
    System::Double Transactions[24][32] = { 0. };
    for (System::Int32 i = 0; i < 24; i++)
    {
        for (System::Int32 j = 0; j < 32; j++)
        {
            Transactions[i][j] = CalcValue(Values, i, j);
        }
    }
    //System::Console::WriteLine(DumpTransactions(Transactions));
    //return 0;
    //System::Console::WriteLine(DumpValues(Values));
    //System::Diagnostics::Debug::WriteLine(args[1]);
    array<System::String^>^ names = System::IO::Directory::GetFiles(args[1], args[2]);
    System::Text::RegularExpressions::Regex^ regex =
        gcnew System::Text::RegularExpressions::Regex(L"([^,]*,){10}\"(" + args[3] + L")\",([^,]*,){85}");
        //^\("\?[^",]*"\?,\?\)\{97\}$
    //System::Diagnostics::Debug::WriteLine(regex->ToString());
    System::Int32 Freq[24][32] = { 0 };
    System::Collections::Generic::Dictionary<System::String^, ref struct DataType^>^ values =
        gcnew System::Collections::Generic::Dictionary<System::String^, ref struct DataType^>();
    ref struct DataType^ All = gcnew ref struct DataType();
    for each (System::String ^ s in names)
    {
        System::Diagnostics::Debug::WriteLine(s);
        rdr = (System::IO::TextReader^) gcnew System::IO::StreamReader(s);
        System::Byte val = 0;
        while ((line = rdr->ReadLine()) != nullptr)
        {
            System::Text::RegularExpressions::Match^ m = regex->Match(line);
            if (m->Length == 0) continue;
            //System::Diagnostics::Debug::WriteLine(DumpMatch(m));
            System::Boolean batterEvent = m->Groups[3]->Captures[35 - 11]->Value == "\"T\",";
            if (!batterEvent) continue;
            System::Boolean First = m->Groups[3]->Captures[26 - 11]->Value != "\"\",";
            System::Boolean Second = m->Groups[3]->Captures[27 - 11]->Value != "\"\",";
            System::Boolean Third = m->Groups[3]->Captures[28 - 11]->Value != "\"\",";
            System::Byte BasesStart = (Third ? 0x04 : 0) |
                                      (Second ? 0x02 : 0) |
                                      (First ? 0x01 : 0);
            System::Byte OutsStart = (m->Groups[1]->Captures[4]->Value[0]-'0');
            First = m->Groups[3]->Captures[58 - 11]->Value == "1," ||
                    m->Groups[3]->Captures[59 - 11]->Value == "1,";
            Second = m->Groups[3]->Captures[58 - 11]->Value == "2," ||
                     m->Groups[3]->Captures[59 - 11]->Value == "2," ||
                     m->Groups[3]->Captures[60 - 11]->Value == "2,";
            Third = m->Groups[3]->Captures[58 - 11]->Value == "3," ||
                    m->Groups[3]->Captures[59 - 11]->Value == "3," ||
                    m->Groups[3]->Captures[60 - 11]->Value == "3," ||
                    m->Groups[3]->Captures[61 - 11]->Value == "3,";
            System::Byte BasesEnd = (Third ? 0x04 : 0) |
                                    (Second ? 0x02 : 0) |
                                    (First ? 0x01 : 0);
            System::Byte OutsEnd = OutsStart + (m->Groups[3]->Captures[40-11]->Value[0]-'0');
            System::Diagnostics::Debug::Assert(((OutsStart << 3) | BasesStart) < 24);
            System::Diagnostics::Debug::Assert(((OutsEnd << 3) | BasesEnd) < 32);
            Freq[(OutsStart<<3) | BasesStart][(OutsEnd<<3) | BasesEnd]++;
            All->n++;
            All->total += Transactions[(OutsStart << 3) | BasesStart][(OutsEnd << 3) | BasesEnd];
            if (!values->ContainsKey(m->Groups[2]->Captures[0]->Value))
            {
                values->Add(m->Groups[2]->Captures[0]->Value, gcnew ref struct DataType());
            }
            values[m->Groups[2]->Captures[0]->Value]->n++;
            values[m->Groups[2]->Captures[0]->Value]->total +=
                    Transactions[(OutsStart << 3) | BasesStart][(OutsEnd << 3) | BasesEnd];
        }
        rdr->Close();
    }
    //System::Diagnostics::Debug::WriteLine (DumpFreq(Freq));
    System::Console::WriteLine("All\t" + All->ToString());
    for each (System::Collections::Generic::KeyValuePair<System::String^,
                                                         ref struct DataType^>^ kp in values)
    {
        System::Console::WriteLine(kp->Key + "\t" + kp->Value->ToString());
    }
    return 0;
}
System::Double CalcValue(System::Double v[][8], System::Int32 from, System::Int32 to)
{
    System::Int32 avail = 1;
    System::Int32 f = from;
    System::Int32 t = to;
    if ((to >> 3) < (from >> 3)) return 0.;
    for (System::Int32 i = 0; avail >= 0 && i < 3; i++)
    {
        avail += (f & 1);
        avail -= (t & 1);
        f >>= 1;
        t >>= 1;
    }
    if (avail + (from>>3) - (to>>3) < 0) return 0.;
    return (System::Double)(avail + f - t) + v[to >> 3][to & 7] - v[from >> 3][from & 7];
}
System::String^ DumpTransactions(System::Double t[][32])
{
    System::String^ retval = "";
    for (System::Int32 i = 0; i < 24; i++)
    {
        for (System::Int32 j = 0; j < 32; j++)
        {
            retval += t[i][j].ToString("F3");
            retval += "\t";
        }
        retval += System::Environment::NewLine;
    }
    return retval;
}
System::String^ DumpValues(System::Double v[][8])
{
    System::String^ retval = "";
    for (System::Int32 i = 0; i < 4; i++)
    {
        for (System::Int32 j = 0; j < 8; j++)
        {
            retval += v[i][j].ToString();
            retval += " ";
        }
        retval += System::Environment::NewLine;
    }
    return retval;
}
System::String^ DumpFreq(System::Int32 v[][32])
{
    System::String^ retval = "";
    for (System::Int32 i = 0; i < 24; i++)
    {
        for (System::Int32 j = 0; j < 32; j++)
        {
            retval += v[i][j].ToString();
            retval += " ";
        }
        retval += System::Environment::NewLine;
    }
    return retval;
}
System::String^ DumpMatch(System::Text::RegularExpressions::Match^ m)
{
    System::String^ retval = "";
    for each (System::Text::RegularExpressions::Group ^ g in m->Groups)
    {
        retval += "Group" + System::Environment::NewLine;
        for each (System::Text::RegularExpressions::Capture ^ c in g->Captures)
        {
            retval += "Capture :" + c->Value + System::Environment::NewLine;
        }
    }
    return retval;
}
/*
* 
* 
number    field
------    -----
0    game id*
1    visiting team*
2    inning*
3    batting team*
4    outs*  !!!
5    balls*
6    strikes*
7    pitch sequence
8    vis score*
9    home score*
10   batter !!!
11   batter hand
12   res batter*
13   res batter hand*
14   pitcher
15   pitcher hand
16   res pitcher*
17   res pitcher hand*
18   catcher
19   first base
20   second base
21   third base
22   shortstop
23   left field
24   center field
25   right field
26   first runner*  !!!
27   second runner* !!!
28   third runner*  !!!
29   event text*
30   leadoff flag*
31   pinchhit flag*
32   defensive position*
33   lineup position*
34   event type*
35   batter event flag*  !!!
36   ab flag*
37   hit value*
38   SH flag*
39   SF flag*
40   outs on play*   !!!
41   double play flag
42   triple play flag
43   RBI on play*
44   wild pitch flag*
45   passed ball flag*
46   fielded by
47   batted ball type
48   bunt flag
49   foul flag
50   hit location
51   num errors*
52   1st error player
53   1st error type
54   2nd error player
55   2nd error type
56   3rd error player
57   3rd error type
58   batter dest* (5 if scores and unearned, 6 if team unearned) !!!
59   runner on 1st dest* (5 if scores and unearned, 6 if team unearned) !!!
60   runner on 2nd dest* (5 if scores and unearned, 6 if team unearned) !!!
61   runner on 3rd dest* (5 if socres and uneanred, 6 if team unearned) !!!
62   play on batter
63   play on runner on 1st
64   play on runner on 2nd
65   play on runner on 3rd
66   SB for runner on 1st flag
67   SB for runner on 2nd flag
68   SB for runner on 3rd flag
69   CS for runner on 1st flag
70   CS for runner on 2nd flag
71   CS for runner on 3rd flag
72   PO for runner on 1st flag
73   PO for runner on 2nd flag
74   PO for runner on 3rd flag
75   Responsible pitcher for runner on 1st
76   Responsible pitcher for runner on 2nd
77   Responsible pitcher for runner on 3rd
78   New Game Flag
79   End Game Flag
80   Pinch-runner on 1st
81   Pinch-runner on 2nd
82   Pinch-runner on 3rd
83   Runner removed for pinch-runner on 1st
84   Runner removed for pinch-runner on 2nd
85   Runner removed for pinch-runner on 3rd
86   Batter removed for pinch-hitter
87   Position of batter removed for pinch-hitter
88   Fielder with First Putout (0 if none)
89   Fielder with Second Putout (0 if none)
90   Fielder with Third Putout (0 if none)
91   Fielder with First Assist (0 if none)
92   Fielder with Second Assist (0 if none)
93   Fielder with Third Assist (0 if none)
94   Fielder with Fourth Assist (0 if none)
95   Fielder with Fifth Assist (0 if none)
96   event num
*/