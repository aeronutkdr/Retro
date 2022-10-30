// RetroUT.cpp : main project file.

#include "stdafx.h"

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
    System::Diagnostics::Trace::Assert (numRunners >= numOuts);
    System::Byte retRuns = numRunners-numOuts;
    return retRuns;
}

int main(array<System::String ^> ^args)
{
    System::IO::TextReader^ rdr   = gcnew System::IO::StreamReader
                                      ("C:\\Users\\Kevin\\My Documents\\"
                                        "retrosheet\\data\\"
                                        "2011WAS_All.txt");
                                        //"2011WAS_One.txt");
    Retro::StringList^ PlayerList = gcnew Retro::StringList();
    Retro::StringList^ TeamList   = gcnew Retro::StringList();
    Retro::StringList^ GameList   = gcnew Retro::StringList();
    System::String^ evt;
    System::Int32 LastGame = -1;
    System::Int32 NumRuns = 0;
    array<System::Double,2>^ freq = gcnew array<System::Double,2>(64,64);
    int i=0;
    while ( (evt=rdr->ReadLine()) != nullptr)
    {
        i++;
        Retro::Event^ e = gcnew Retro::Event(evt,
                                             PlayerList,
                                             GameList,
                                             TeamList,
                                             true);
        System::Byte from = e->From();
        System::Byte to   = e->To();
        freq[from,to]++;
        if (e->RunnersLost() > 0)
        {
            System::Console::WriteLine (e->RunnersLost() + "\t" + evt);
        }
        if (LastGame != e->GameID())
        {
            System::Console::WriteLine ("Game #" + LastGame + "\t" + NumRuns);
            LastGame = e->GameID();
        }
        System::Byte r = Runs (from, to);
        //if (r > 0) System::Console::WriteLine (evt + "\t" + r);
        NumRuns += Runs (from, to);
    }
    rdr->Close();
    System::Console::WriteLine ("Game #" + LastGame + "\t" + NumRuns);


    return 0;
}
