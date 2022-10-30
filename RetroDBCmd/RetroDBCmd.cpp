// RetroDBCmd.cpp : main project file.

#include "stdafx.h"

using namespace System;

int main(array<System::String ^> ^args)
{
    System::Data::OleDb::OleDbConnection^ m_DB =
        gcnew System::Data::OleDb::OleDbConnection
            ("Provider=Microsoft.Jet.OLEDB.4.0;"
             "Data Source=RetroDB.accdb");
    m_DB->Open();

    Retro::DBDictionary^ Files   = gcnew Retro::DBDictionary (m_DB, "Files");
    Retro::DBDictionary^ Games   = gcnew Retro::DBDictionary (m_DB, "Games");
    Retro::DBDictionary^ Players = gcnew Retro::DBDictionary (m_DB, "Players");
    Retro::DBDictionary^ Teams   = gcnew Retro::DBDictionary (m_DB, "Teams");

    System::IO::DirectoryInfo^ Dir = gcnew System::IO::DirectoryInfo
                              ("..\\..\\..\\..\\retrosheet\\data");
    array<System::IO::FileInfo^>^ DirFiles = Dir->GetFiles("*.txt");
    for each (System::IO::FileInfo^ file in DirFiles)
    {
        System::Console::WriteLine ("Reading: " + file->Name);
        System::Int32 ^FileID;
        /* fill [File] */
        if (Files->Add (file->Name, FileID))
        {
            /* fill [FileData] */
            System::Console::WriteLine ("ID = " + *FileID +
                                        " Update [FileData]: " + file->Name);
            System::Int32 year = System::Convert::ToInt32 (file->Name->Substring(0,4));
            System::String^ team = file->Name->Substring(4,3);
            System::Int32 tID = Teams->Add (team);
            System::Data::OleDb::OleDbCommand^ aInsCmd = m_DB->CreateCommand();
            aInsCmd->CommandText = "INSERT INTO"
                                   " [FileData] ("
                                   " [ID]"
                                   " [RetroYear]"
                                   " [HomeTeamID]"
                                   ")"
                                   " VALUES"
                                   " (" + *FileID +
                                   "," + year + 
                                   "," + tID + ")";
            System::Diagnostics::Debug::Print (aInsCmd->CommandText);
            System::Diagnostics::Trace::Assert (aInsCmd->ExecuteNonQuery() == 1);
            System::IO::TextReader^ rdr = gcnew System::IO::StreamReader
                                                                  (file->FullName);
            System::String^ evt;
            while ( (evt=rdr->ReadLine()) != nullptr)
            {
                Retro::DBEvent^ e = gcnew Retro::DBEvent (*FileID,
                                                          evt,
                                                          Players,
                                                          Games,
                                                          Teams);
            }
        }
#if 0
        System::IO::TextReader^ rdr = gcnew System::IO::StreamReader
                                                              (file->FullName);
        System::String^ evt;
        while ( (evt=rdr->ReadLine()) != nullptr)
        {
            Retro::Event^ e = gcnew Retro::Event(evt,
                                                 Players,
                                                 Games,
                                                 Teams,
                                                 false);
            System::Byte from = e->From();
            System::Byte to   = e->To();
        }
        rdr->Close();
#endif
    }
    return 0;
}
