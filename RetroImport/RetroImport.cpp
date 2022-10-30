// RetroImport.cpp : main project file.

#include "stdafx.h"

using namespace System;

int main(array<System::String ^> ^args)
{
    if (args->Length != 3)
    {
        System::Console::WriteLine ("RetroImport.exe <database> <topdir> <pattern>");
        System::Console::WriteLine ("RetroImport.exe Retrosheet C:\\Users\\Kevin\\Documents\\retrosheet\\unzipped 2016*.ev?");
        System::Console::WriteLine ("This program will:");
        System::Console::WriteLine ("1. open the <database>");
        System::Console::WriteLine ("2. for all files in <topdir>\\<pattern>:");
        System::Console::WriteLine ("  a. open the file");
        System::Console::WriteLine ("  b. calculate CRC");
        System::Console::WriteLine ("  c. if this file is unknown from the database:");
        System::Console::WriteLine ("    1. add the file to the database");
        System::Console::WriteLine ("  d. fetch the last known CRC from the database");
        System::Console::WriteLine ("  e. if CRC mismatch:");
        System::Console::WriteLine ("    1. delete all events sourced from this file");
        System::Console::WriteLine ("    2. run bevent.exe on this file");
        System::Console::WriteLine ("    3. copy all events to the database");
        System::Console::WriteLine ("3. close the <database>");
        return -1;
    }
    System::String^ DBName = L"Database=" + args[0] + L";"
                             L"Trusted_Connection=yes;"
                             L"Server=BABYG\\SQLEXPRESS;"
                             L"Provider=SQLOLEDB";
    System::String^ TblName = L"[" + args[0] + L"].[dbo].[rawFiles]";
    System::String^ EvtTblName = L"[" + args[0] + L"].[dbo].[rawEvents]";

    System::Data::OleDb::OleDbConnection^ m_DB =
                            gcnew System::Data::OleDb::OleDbConnection (DBName);
    m_DB->Open();

    array<System::String^>^ files = System::IO::Directory::GetFiles
                                                            (args[1],
                                                             args[2],
                                                             System::IO::SearchOption::AllDirectories);
    for each (System::String^ file in files)
    {
        System::IO::BinaryReader^ brdr = gcnew System::IO::BinaryReader(System::IO::File::OpenRead(file));
        array<System::Byte>^ bytes = brdr->ReadBytes((System::Int32) brdr->BaseStream->Length);
        System::Int32 CRC = CRC32::CRC32::calc(bytes);
        brdr->Close();

        System::Data::OleDb::OleDbCommand^ aCmd = m_DB->CreateCommand();
        file = file->Substring(file->LastIndexOf('\\')+1);
        aCmd->CommandText = L"SELECT " +
                            TblName + L".[ID], " +
                            TblName + L".[CRC] " +
                            L"FROM " + TblName +
                            L" WHERE " +
                            TblName +L".[Name]='" + file + "'";
        //System::Diagnostics::Debug::WriteLine (aCmd->CommandText);
        System::Data::OleDb::OleDbDataReader^ rdr = aCmd->ExecuteReader();
        System::Int32 DBCRC;
        System::Int32 DBID;
        if (!rdr->Read())
        {
            rdr->Close();
            System::Data::OleDb::OleDbCommand^ insCmd = m_DB->CreateCommand();
            insCmd->CommandText = L"INSERT INTO " + TblName +
                                  L" ([Name], [CRC])"
                                  L" VALUES"
                                  L" ('" + file + "'," +
                                  (CRC-1) + ")";
            //System::Diagnostics::Debug::WriteLine (insCmd->CommandText);
            insCmd->ExecuteNonQuery();
            //System::Diagnostics::Debug::WriteLine (aCmd->CommandText);
            rdr = aCmd->ExecuteReader();
            rdr->Read();
        }
        DBID  = rdr->GetInt32(0);
        DBCRC = rdr->GetInt32(1);
        rdr->Close();
        if (CRC != DBCRC)
        {
            System::Data::OleDb::OleDbCommand^ delCmd = m_DB->CreateCommand();
            delCmd->CommandText = L"DELETE FROM " + EvtTblName +
                                  L" WHERE [rawFileID]=" + DBID;
            //System::Diagnostics::Debug::WriteLine (delCmd->CommandText);
            delCmd->ExecuteNonQuery();
            System::Diagnostics::ProcessStartInfo^ ps =
                gcnew System::Diagnostics::ProcessStartInfo (args[1] + L"\\BEVENT.EXE");
            ps->CreateNoWindow         = true;
            ps->RedirectStandardOutput = true;
            ps->UseShellExecute        = false;
            ps->WorkingDirectory       = args[1];
            ps->Arguments = L"-y " +
                             file->Substring(0,4) +
                             L" -f 0-96 " +
                             file;
            System::Diagnostics::Process ^p = System::Diagnostics::Process::Start (ps);
            System::String^ str = p->StandardOutput->ReadToEnd();
            p->WaitForExit();
            //System::Diagnostics::Debug::WriteLine(str);
            array<System::String^>^ delimit = gcnew array<System::String^>(1);
            delimit[0] = System::Environment::NewLine;
            array<System::String^>^ lines = str->Split(delimit, System::StringSplitOptions::RemoveEmptyEntries);
            System::IO::TextWriter^ wtr = gcnew System::IO::StreamWriter(args[1] + "\\" + file + ".out");
            for each (System::String^ str in lines)
            {
                wtr->WriteLine(DBID + "," + str);
            }
            wtr->Close();
            ps->FileName = L"bcp";
            ps->Arguments = L"Retrosheet.dbo.rawEvents in " +
                            file + L".out" +
                            L" -T -f rawEvents.xml -S BABYG\\SQLEXPRESS -t , -r 0x0D0A -e err\\" +
                            file +
                            L"_err.txt";
            p = System::Diagnostics::Process::Start (ps);
            str = p->StandardOutput->ReadToEnd();
            p->WaitForExit();
            System::Diagnostics::Debug::WriteLine(str);
            System::String^ rows = str->Substring(0,str->IndexOf(" rows copied"));
            rows = rows->Substring(rows->LastIndexOf('\n')+1);
            System::String^ clockTime = str->Substring(str->IndexOf("Clock Time (ms.) Total") + 29);
            clockTime = clockTime->Substring(0,clockTime->IndexOf(' '));
            System::Data::OleDb::OleDbCommand^ insCmd = m_DB->CreateCommand();
            insCmd->CommandText = L"UPDATE " + TblName +
                                  L" SET [CRC]=" + CRC + L"," +
                                      L" [ProcessingTime]=" + clockTime + L","
                                      L" [NumRows]=" + rows +
                                  L" WHERE [ID]=" + DBID;
            //System::Diagnostics::Debug::WriteLine (insCmd->CommandText);
            insCmd->ExecuteNonQuery();
            //5965 rows copied.
            //Network packet size (bytes): 4096
            //Clock Time (ms.) Total     : 375    Average : (15906.67 rows per sec.)
            System::Console::WriteLine (file + ": processed WAS: " + DBCRC.ToString("X") + " IS: " + CRC.ToString("X") );
        }
        else
        {
            System::Console::WriteLine (file + ": skipped" );
        }
    }
    m_DB->Close();
    return 0;
}
