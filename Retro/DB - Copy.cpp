#include "StdAfx.h"
#include "DB.h"

using namespace Retro;

DB::DB(void)
{
}

DB::DB(System::String^ connection)
{
    m_DB = gcnew System::Data::OleDb::OleDbConnection (connection);
    m_DB->Open();
}

System::Void DB::Close (System::Void)
{
    m_DB->Close();
}

System::Boolean DB::Add (System::String^ file, FileEvent^ evt)
{
    System::Int32 fileID  = AddFile (file);
    System::Int32 gameID  = AddGame (evt->m_GameName, fileID, evt->m_VisitTeam);
    System::Int32 Player0 = AddSimple ("Players", evt->m_BatterStart);
    System::Int32 Player1 = AddSimple ("Players", evt->m_Runner1);
    System::Int32 Player2 = AddSimple ("Players", evt->m_Runner2);
    System::Int32 Player3 = AddSimple ("Players", evt->m_Runner3);
    System::Byte from     = evt->From();
    System::Byte to       = evt->To();
    System::Diagnostics::Trace::Assert (m_DB->State == System::Data::ConnectionState::Open);
    System::Data::OleDb::OleDbCommand^ aInsCmd = m_DB->CreateCommand();
    aInsCmd->CommandText = "INSERT INTO"
                           " [Events] ("
                           "[GameID]"
                           ",[Inning]"
                           ",[HomeTeamBatting]"
                           ",[Base0_ID]"
                           ",[Base1_ID]"
                           ",[Base2_ID]"
                           ",[Base3_ID]"
                           ",[Start]"
                           ",[End]"
                           ")"
                           " VALUES"
                           " (" +
                           "" + gameID +
                           "," + evt->m_Inning +
                           "," + (evt->m_HomeTeamBatting?"-1":"0") +
                           "," + Player0 +
                           "," + Player1 +
                           "," + Player2 +
                           "," + Player3 +
                           "," + from +
                           "," + to +
                           ")";
    //System::Diagnostics::Debug::Print (aInsCmd->CommandText);
    System::Diagnostics::Trace::Assert (aInsCmd->ExecuteNonQuery() == 1);
    return true;
}

System::Int32 DB::AddGame (System::String^ game,
                           System::Int32   fileID,
                           System::String^ visitTeam)
{
    System::Diagnostics::Trace::Assert (m_DB->State == System::Data::ConnectionState::Open);
    System::Data::OleDb::OleDbCommand^ aCmd = m_DB->CreateCommand();
    aCmd->CommandText = "SELECT"
                        " [Games].[ID]"
                        " FROM [Games]"
                        " WHERE"
                        " [Games].[RetroName]='" + game + "'";
    //System::Diagnostics::Debug::Print (aCmd->CommandText);
    System::Int32^ ID = (System::Int32^) aCmd->ExecuteScalar();
    if (ID == nullptr)
    {
        /* WAS201103310 */
        System::DateTime^ date = gcnew System::DateTime
                           (System::Convert::ToInt32 (game->Substring(3,4)),
                            System::Convert::ToInt32 (game->Substring(7,2)),
                            System::Convert::ToInt32 (game->Substring(9,2)));
        System::Int32 seq = System::Convert::ToInt32 (game->Substring(11,1));
        System::Data::OleDb::OleDbCommand^ aInsCmd = m_DB->CreateCommand();
        System::Int32 teamID = AddSimple ("Teams", visitTeam);
        aInsCmd->CommandText = "INSERT INTO"
                               " [Games] ("
                               "[RetroName]"
                               ",[FileID]"
                               ",[VisitingTeamID]"
                               ",[SequenceNumber]"
                               ",[RetroDate]"
                               ")"
                               " VALUES"
                               " ("
                               "'" + game + "'" +
                               "," + fileID +
                               "," + teamID +
                               "," + seq +
                               ",'" + date->ToString("d") + "'" +
                               ")";
        //System::Diagnostics::Debug::Print (aInsCmd->CommandText);
        System::Diagnostics::Trace::Assert (aInsCmd->ExecuteNonQuery() == 1);
        ID = (System::Int32^) aCmd->ExecuteScalar();
        System::Diagnostics::Trace::Assert (ID != nullptr);
    }
    return *ID;
}

System::Int32 DB::AddFile (System::String^ file)
{
    System::Diagnostics::Trace::Assert (m_DB->State == System::Data::ConnectionState::Open);
    System::Data::OleDb::OleDbCommand^ aCmd = m_DB->CreateCommand();
    aCmd->CommandText = "SELECT"
                        " [Files].[ID]"
                        " FROM [Files]"
                        " WHERE"
                        " [Files].[RetroName]='" + file + "'";
    //System::Diagnostics::Debug::Print (aCmd->CommandText);
    System::Int32^ ID = (System::Int32^) aCmd->ExecuteScalar();
    if (ID == nullptr)
    {
        System::Int32   fileyear   = System::Convert::ToInt32 (file->Substring(0,4));
        System::String^ fileteam   = file->Substring(4,3);
        System::String^ fileleague = file->Substring(8,3);
        System::Int32 teamID = AddSimple ("Teams", fileteam);
        System::Int32 leagueID = AddSimple ("Leagues", fileleague);
        System::Data::OleDb::OleDbCommand^ aInsCmd = m_DB->CreateCommand();
        aInsCmd->CommandText = "INSERT INTO"
                            " [Files] ("
                            "[RetroName]"
                            ",[RetroYear]"
                            ",[HomeTeamID]"
                            ",[LeagueID]"
                            ")"
                            " VALUES"
                            " ("
                            "'" + file + "'" +
                            "," + fileyear +
                            "," + teamID +
                            "," + leagueID +
                            ")";
        System::Diagnostics::Debug::Print (aInsCmd->CommandText);
        System::Diagnostics::Trace::Assert (aInsCmd->ExecuteNonQuery() == 1);
        ID = (System::Int32^) aCmd->ExecuteScalar();
        System::Diagnostics::Trace::Assert (ID != nullptr);
    }
    return *ID;
}

System::Int32 DB::AddSimple (System::String^ table,
                             System::String^ name)
{
    if (System::String::IsNullOrEmpty(name)) return 0;
    System::Diagnostics::Trace::Assert (m_DB->State == System::Data::ConnectionState::Open);
    System::Data::OleDb::OleDbCommand^ aCmd = m_DB->CreateCommand();
    aCmd->CommandText = "SELECT"
                        " [" + table + "].[ID]"
                        " FROM [" + table + "]"
                        " WHERE"
                        " [" + table + "].[RetroName]='" + name + "'";
    //System::Diagnostics::Debug::Print (aCmd->CommandText);
    System::Int32^ ID = (System::Int32^) aCmd->ExecuteScalar();
    if (ID == nullptr)
    {
        System::Data::OleDb::OleDbCommand^ aInsCmd = m_DB->CreateCommand();
        aInsCmd->CommandText = "INSERT INTO"
                            " [" + table + "] ("
                            "[RetroName]"
                            ")"
                            " VALUES"
                            " ('" + name + "')";
        System::Diagnostics::Debug::Print (aInsCmd->CommandText);
        System::Diagnostics::Trace::Assert (aInsCmd->ExecuteNonQuery() == 1);

        ID = (System::Int32^) aCmd->ExecuteScalar();
        System::Diagnostics::Trace::Assert (ID != nullptr);
    }
    return *ID;
}

System::Void DB::Clean (System::Void)
{
    System::Data::OleDb::OleDbCommand^ aCmd = m_DB->CreateCommand();
    aCmd->CommandText = "DELETE * FROM [Teams]"  ; aCmd->ExecuteNonQuery();
    aCmd->CommandText = "DELETE * FROM [Players]"; aCmd->ExecuteNonQuery();
    aCmd->CommandText = "DELETE * FROM [Leagues]"; aCmd->ExecuteNonQuery();
    aCmd->CommandText = "DELETE * FROM [Events]" ; aCmd->ExecuteNonQuery();
    aCmd->CommandText = "DELETE * FROM [Games]"  ; aCmd->ExecuteNonQuery();
    aCmd->CommandText = "DELETE * FROM [Files]"  ; aCmd->ExecuteNonQuery();
}
