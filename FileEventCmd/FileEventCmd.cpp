// FileEventCmd.cpp : main project file.

#include "stdafx.h"

using namespace System;

int main(array<System::String ^> ^args)
{
    System::IO::DirectoryInfo^ Dir = gcnew System::IO::DirectoryInfo
                              ("..\\..\\..\\..\\retrosheet\\data");
    //System::Collections::Generic::List<Retro::FileEvent^>^ FileEvents =
    //    gcnew System::Collections::Generic::List<Retro::FileEvent^>;
    //array<System::IO::FileInfo^>^ DirFiles = Dir->GetFiles("2011*.txt");
    //array<System::IO::FileInfo^>^ DirFiles = Dir->GetFiles("2010*.txt");
    array<System::IO::FileInfo^>^ DirFiles = Dir->GetFiles("2009*.txt");
    //array<System::IO::FileInfo^>^ DirFiles = Dir->GetFiles("2011NYN_EVN.txt");
    Retro::DB ^aDB = gcnew Retro::DB("Provider=Microsoft.ACE.OLEDB.12.0;"
                                     //"Provider=Microsoft.Jet.OLEDB.4.0;"
                                     "Data Source=Retro.accdb");
    //aDB->Clean();
    //return 0;
    for each (System::IO::FileInfo^ file in DirFiles)
    {
        if (!aDB->FileExists(file->Name))
        {
            System::Console::WriteLine ("Reading: " + file->Name);
            System::IO::TextReader^ rdr = gcnew System::IO::StreamReader
                                                              (file->FullName);
            System::String^ evt;
            while ( (evt=rdr->ReadLine()) != nullptr)
            {
                Retro::FileEvent^ e = gcnew Retro::FileEvent(evt);
                System::Diagnostics::Trace::Assert (aDB->Add(file->Name, e));
                //FileEvents->Add (gcnew Retro::FileEvent(evt));
            }
            rdr->Close();
        }
    }
    aDB->Close();
    //System::Console::WriteLine ("Number of Events: " + FileEvents->Count);
    //for each (Retro::FileEvent ^e in FileEvents)
    //{
    //    System::Console::Write (e->ToString());
    //}
    //FileEvents->Clear();
    return 0;
}
