#include "StdAfx.h"
#include "RetroDB.h"

Retro::RetroDB::RetroDB(System::String^ connection, System::Int32 PreloadMask)
{
    //Address=(local);
    //Database=Retro;
    //Trusted_Connection=yes;
    //Provider=SQLOLEDB
    m_DB = gcnew System::Data::OleDb::OleDbConnection (connection);
    m_DB->Open();

    Files   = gcnew Retro::DBDictionary (m_DB,
                                         "Files",
                                         (PreloadMask & (System::Int32) EntityType::Files) ==
                                          (System::Int32) EntityType::Files);
    Games   = gcnew Retro::DBDictionary (m_DB,
                                         "Games",
                                         (PreloadMask & (System::Int32) EntityType::Games) ==
                                          (System::Int32) EntityType::Games);
    Players = gcnew Retro::DBDictionary (m_DB,
                                         "Players",
                                         (PreloadMask & (System::Int32) EntityType::Players) ==
                                          (System::Int32) EntityType::Players);
    Teams   = gcnew Retro::DBDictionary (m_DB,
                                         "Teams",
                                         (PreloadMask & (System::Int32) EntityType::Teams) ==
                                          (System::Int32) EntityType::Teams);
}

//System::Boolean Retro::RetroDB::AddEvents (System::String^ name)
//{
//    System::Int32^ FileID;
//    System::Boolean retval = AddEventFile (name, FileID);
//    if (retval)
//    {
//    }
//    return retval;
//}

System::Boolean Retro::RetroDB::AddEventFile (System::String^ name,
                                              System::Int32%  fID,
                                              System::Int32 crc)
{
    System::Boolean retval = Files->Add (name, fID);
    if (retval)
    {
        System::String^ HomeTeam = name->Substring(4,3);
        System::Int32 htID = Teams->Add (HomeTeam);
        System::Diagnostics::Trace::Assert
                    (m_DB->State == System::Data::ConnectionState::Open);
        System::Data::OleDb::OleDbCommand^ aCmd = m_DB->CreateCommand();
        aCmd->CommandText = "UPDATE "
                            "[" + Files->m_Table + "]"
                            " SET [HomeTeamID]=" + htID +
                            " ,[CRC]=" + crc +
                            " WHERE [ID]=" + fID;
        //System::Diagnostics::Debug::Print (aCmd->CommandText);
        System::Diagnostics::Trace::Assert (aCmd->ExecuteNonQuery() == 1);
    }
    return retval;
}

System::Boolean Retro::RetroDB::AddRetroEvent (System::Int32 fID, Retro::RetroEvent ^e)
{
    System::Int32 vID = Teams->Add (e->m_Team);

    System::Int32 GameID           = AddGame (e->m_Game, fID, vID);
    System::Int32 BatterID         = Players->Add (e->m_BatterStart);
    System::Int32 ResBatterID      = Players->Add (e->m_BatterRes);
    System::Int32 PitcherID        = Players->Add (e->m_PitcherStart);
    System::Int32 ResPitcherID     = Players->Add (e->m_PitcherRes);
    System::Int32 CatcherID        = Players->Add (e->m_Catcher);
    System::Int32 FirstBaseID      = Players->Add (e->m_First);
    System::Int32 SecondBaseID     = Players->Add (e->m_Second);
    System::Int32 ThirdBaseID      = Players->Add (e->m_Third);
    System::Int32 ShortstopID      = Players->Add (e->m_Shortstop);
    System::Int32 LeftFieldID      = Players->Add (e->m_Left);
    System::Int32 CenterFieldID    = Players->Add (e->m_Center);
    System::Int32 RightFieldID     = Players->Add (e->m_Right);
    System::Int32 Runner1BID       = Players->Add (e->m_1B);
    System::Int32 Runner2BID       = Players->Add (e->m_2B);
    System::Int32 Runner3BID       = Players->Add (e->m_3B);
    System::Int32 PitcherResp1ID   = Players->Add (e->m_PitcherResp1);
    System::Int32 PitcherResp2ID   = Players->Add (e->m_PitcherResp2);
    System::Int32 PitcherResp3ID   = Players->Add (e->m_PitcherResp3);
    System::Int32 Runner1RemovedID = Players->Add (e->m_Runner1Removed);
    System::Int32 Runner2RemovedID = Players->Add (e->m_Runner2Removed);
    System::Int32 Runner3RemovedID = Players->Add (e->m_Runner3Removed);
    System::Int32 BatterRemovedID  = Players->Add (e->m_BatterRemoved);

    System::Data::OleDb::OleDbCommand^ aInsCmd = m_DB->CreateCommand();
    aInsCmd->CommandText = "INSERT INTO"
                           " [Events] ("
                           "[GameID]"
                           ",[Inning]"
                           ",[BattingTeam]"
                           ",[Outs]"
                           ",[Balls]"
                           ",[Strikes]"
                           ",[PitchSeq]"
                           ",[VisitorScore]"
                           ",[HomeScore]"
                           ",[BatterID]"
                           ",[BatterHandRight]"
                           ",[ResBatterID]"
                           ",[ResBatterHandRight]"
                           ",[PitcherID]"
                           ",[PitcherHandRight]"
                           ",[ResPitcherID]"
                           ",[ResPitcherHandRight]"
                           ",[CatcherID]"
                           ",[FirstBaseID]"
                           ",[SecondBaseID]"
                           ",[ThirdBaseID]"
                           ",[ShortstopID]"
                           ",[LeftFieldID]"
                           ",[CenterFieldID]"
                           ",[RightFieldID]"
                           ",[Runner1BID]"
                           ",[Runner2BID]"
                           ",[Runner3BID]"
                           ",[EventText]"
                           ",[Leadoff]"
                           ",[PinchHit]"
                           ",[DefensePos]"
                           ",[LineupPos]"
                           ",[EventTypeID]"
                           ",[BatterEvent]"
                           ",[AB]"
                           ",[HitValue]"
                           ",[SH]"
                           ",[SF]"
                           ",[OutsOnPlay]"
                           ",[DoublePlay]"
                           ",[TriplePlay]"
                           ",[RBI]"
                           ",[WildPitch]"
                           ",[PassedBall]"
                           ",[FieldPos]"
                           ",[BattedBall]"
                           ",[Bunt]"
                           ",[Foul]"
                           ",[HitLocation]"
                           ",[NumErrors]"
                           ",[Error1Player]"
                           ",[Error1Type]"
                           ",[Error2Player]"
                           ",[Error2Type]"
                           ",[Error3Player]"
                           ",[Error3Type]"
                           ",[DestBatter]"
                           ",[DestRunner1]"
                           ",[DestRunner2]"
                           ",[DestRunner3]"
                           ",[PlayBatter]"
                           ",[PlayRunner1]"
                           ",[PlayRunner2]"
                           ",[PlayRunner3]"
                           ",[SBRunner1]"
                           ",[SBRunner2]"
                           ",[SBRunner3]"
                           ",[CSRunner1]"
                           ",[CSRunner2]"
                           ",[CSRunner3]"
                           ",[PORunner1]"
                           ",[PORunner2]"
                           ",[PORunner3]"
                           ",[PitcherResp1ID]"
                           ",[PitcherResp2ID]"
                           ",[PitcherResp3ID]"
                           ",[GameNew]"
                           ",[GameEnd]"
                           ",[Pinch1]"
                           ",[Pinch2]"
                           ",[Pinch3]"
                           ",[Runner1RemovedID]"
                           ",[Runner2RemovedID]"
                           ",[Runner3RemovedID]"
                           ",[BatterRemovedID]"
                           ",[BatterRemovedPos]"
                           ",[FielderPO1]"
                           ",[FielderPO2]"
                           ",[FielderPO3]"
                           ",[FielderAst1]"
                           ",[FielderAst2]"
                           ",[FielderAst3]"
                           ",[FielderAst4]"
                           ",[FielderAst5]"
                           ",[EventNum]"
                           ",[ValueStart]"
                           ",[ValueEnd]"
                           ",[EvtLineNum]"
                           ")"
                           " VALUES (" +
                           GameID +
                           ","+e->m_Inning +
                           ",'"+e->m_HomeTeamBatting + "'" +
                           ","+e->m_Outs +
                           ","+e->m_Balls +
                           ","+e->m_Strikes +
                           ",'"+e->m_Pitches + "'" +
                           ","+e->m_ScoreVis +
                           ","+e->m_ScoreHome +
                           ","+BatterID +
                           ",'"+e->m_BatsRightStart + "'" +
                           ","+ResBatterID +
                           ",'"+e->m_BatsRightRes + "'" +
                           ","+PitcherID +
                           ",'"+e->m_PitchRightStart + "'" +
                           ","+ResPitcherID +
                           ",'"+e->m_PitchRightRes + "'" +
                           ","+CatcherID +
                           ","+FirstBaseID +
                           ","+SecondBaseID +
                           ","+ThirdBaseID +
                           ","+ShortstopID +
                           ","+LeftFieldID +
                           ","+CenterFieldID +
                           ","+RightFieldID +
                           ","+Runner1BID +
                           ","+Runner2BID +
                           ","+Runner3BID +
                           ",'"+e->m_EventText + "'" +
                           ",'"+e->m_Leadoff + "'" +
                           ",'"+e->m_PinchHit + "'" +
                           ","+e->m_DefensePos +
                           ","+e->m_LineupPos +
                           ","+e->m_EventType +
                           ",'"+e->m_BatterEvent + "'" +
                           ",'"+e->m_AB + "'" +
                           ","+e->m_HitValue +
                           ",'"+e->m_SH + "'" +
                           ",'"+e->m_SF + "'" +
                           ","+e->m_OutsOnPlay +
                           ",'"+e->m_DoublePlay + "'" +
                           ",'"+e->m_TriplePlay + "'" +
                           ","+e->m_RBI +
                           ",'"+e->m_WildPitch + "'" +
                           ",'"+e->m_PassedBall + "'" +
                           ","+e->m_FieldPos +
                           ",'"+e->m_BattedBall + "'" +
                           ",'"+e->m_Bunt + "'" +
                           ",'"+e->m_Foul + "'" +
                           ",'"+e->m_HitLocation + "'" +
                           ","+e->m_NumErrors +
                           ","+e->m_Error1Player +
                           ",'"+e->m_Error1Type + "'" +
                           ","+e->m_Error2Player +
                           ",'"+e->m_Error2Type + "'" +
                           ","+e->m_Error3Player +
                           ",'"+e->m_Error3Type + "'" +
                           ","+e->m_DestBatter +
                           ","+e->m_DestRunner1 +
                           ","+e->m_DestRunner2 +
                           ","+e->m_DestRunner3 +
                           ",'"+e->m_PlayBatter + "'" +
                           ",'"+e->m_PlayRunner1 + "'" +
                           ",'"+e->m_PlayRunner2 + "'" +
                           ",'"+e->m_PlayRunner3 + "'" +
                           ",'"+e->m_SBRunner1 + "'" +
                           ",'"+e->m_SBRunner2 + "'" +
                           ",'"+e->m_SBRunner3 + "'" +
                           ",'"+e->m_CSRunner1 + "'" +
                           ",'"+e->m_CSRunner2 + "'" +
                           ",'"+e->m_CSRunner3 + "'" +
                           ",'"+e->m_PORunner1 + "'" +
                           ",'"+e->m_PORunner2 + "'" +
                           ",'"+e->m_PORunner3 + "'" +
                           ","+PitcherResp1ID +
                           ","+PitcherResp2ID +
                           ","+PitcherResp3ID +
                           ",'"+e->m_GameNew + "'" +
                           ",'"+e->m_GameEnd + "'" +
                           ",'"+e->m_Pinch1 + "'" +
                           ",'"+e->m_Pinch2 + "'" +
                           ",'"+e->m_Pinch3 + "'" +
                           ","+Runner1RemovedID +
                           ","+Runner2RemovedID +
                           ","+Runner3RemovedID +
                           ","+BatterRemovedID +
                           ","+e->m_BatterRemovedPos +
                           ","+e->m_FielderPO1 +
                           ","+e->m_FielderPO2 +
                           ","+e->m_FielderPO3 +
                           ","+e->m_FielderAst1 +
                           ","+e->m_FielderAst2 +
                           ","+e->m_FielderAst3 +
                           ","+e->m_FielderAst4 +
                           ","+e->m_FielderAst5 +
                           ","+e->m_EventNum +
                           ","+e->m_StartValue +
                           ","+e->m_EndValue +
                           ","+e->m_EvtLineNum +
                           ")";
    //System::Diagnostics::Debug::Print (aInsCmd->CommandText);
    return aInsCmd->ExecuteNonQuery() == 1;
    //return true;
}

System::Void Retro::RetroDB::Close (System::Void)
{
    m_DB->Close();
}

System::Int32 Retro::RetroDB::AddGame (System::String^ name,
                                       System::Int32   fID,
                                       System::Int32   vID)
{
    System::Int32 gID;
    if (Games->Add (name, gID))
    {
        //NYA201103310
        System::DateTime gameDate(System::Convert::ToInt32(name->Substring(3,4)),
                                  System::Convert::ToInt32(name->Substring(7,2)),
                                  System::Convert::ToInt32(name->Substring(9,2)));
        System::Int32 Idx = System::Convert::ToInt32(name->Substring(11,1));
        System::Diagnostics::Trace::Assert
                    (m_DB->State == System::Data::ConnectionState::Open);
        System::Data::OleDb::OleDbCommand^ aCmd = m_DB->CreateCommand();
        aCmd->CommandText = "UPDATE "
                            "[" + Games->m_Table + "]"
                            " SET" +
                            " [Date]='" + gameDate.ToString() + "'" +
                            ",[Idx]=" + Idx +
                            ",[VisitTeamID]=" + vID +
                            ",[FileID]=" + fID +
                            " WHERE [ID]=" + gID;
        //System::Diagnostics::Debug::Print (aCmd->CommandText);
        System::Diagnostics::Trace::Assert (aCmd->ExecuteNonQuery() == 1);
    }
    return gID;
}

System::Void Retro::RetroDB::Clear (System::Void)
{
    Files->Clear();
    Games->Clear();
    Players->Clear();
    Teams->Clear();
}

System::Collections::Generic::List<Retro::ResultType^>^ Retro::RetroDB::Query(System::String^ str)
{
    System::Data::OleDb::OleDbCommand^ aCmd = m_DB->CreateCommand();
    aCmd->CommandText = str;
    aCmd->CommandTimeout = 0;
    System::Data::OleDb::OleDbDataReader^ rdr = aCmd->ExecuteReader(System::Data::CommandBehavior::SequentialAccess);
    System::Collections::Generic::List<Retro::ResultType^>^ retval = gcnew System::Collections::Generic::List<Retro::ResultType^>;
    while (rdr->Read())
    {
        Retro::ResultType^ res = gcnew Retro::ResultType;
        res->m_Start = System::Convert::ToInt32 (rdr[0]);
        res->m_End = System::Convert::ToInt32 (rdr[1]);
        res->m_Freq = System::Convert::ToInt32 (rdr[2]);
        retval->Add(res);
    }
    rdr->Close();
    return retval;
}

System::Collections::Generic::List<Retro::StateFreqType^>^ Retro::RetroDB::QueryFreq(System::String^ str)
{
    System::Data::OleDb::OleDbCommand^ aCmd = m_DB->CreateCommand();
    aCmd->CommandText = str;
    aCmd->CommandTimeout = 0;
    System::Data::OleDb::OleDbDataReader^ rdr = aCmd->ExecuteReader(System::Data::CommandBehavior::SequentialAccess);
    System::Collections::Generic::List<Retro::StateFreqType^>^ retval = gcnew System::Collections::Generic::List<Retro::StateFreqType^>;
    while (rdr->Read())
    {
        Retro::StateFreqType^ res = gcnew Retro::StateFreqType;
        res->m_State = System::Convert::ToInt32 (rdr[0]);
        res->m_Freq = System::Convert::ToInt32 (rdr[1]);
        res->m_StartEnd = System::Convert::ToInt32 (rdr[2]);
        retval->Add(res);
    }
    rdr->Close();
    return retval;
}

System::Boolean Retro::RetroDB::UpdateFileCRC (System::String^ name, System::Int32 CRC)
{
    System::Int32 fID;
    System::Boolean retval = Files->Add (name, fID);
    if (!retval)
    {
        System::Diagnostics::Trace::Assert
                    (m_DB->State == System::Data::ConnectionState::Open);
        System::Data::OleDb::OleDbCommand^ aCmd = m_DB->CreateCommand();
        aCmd->CommandText = "UPDATE "
                            "[" + Files->m_Table + "]"
                            " SET [CRC]=" + CRC.ToString() +
                            " WHERE [ID]=" + fID;
        //System::Diagnostics::Debug::Print (aCmd->CommandText);
        System::Diagnostics::Trace::Assert (aCmd->ExecuteNonQuery() == 1);
    }
    return !retval;
}

System::Collections::Generic::List<System::String^>^ Retro::RetroDB::AllPlayers(System::Void)
{
    System::String^ SQL = "SELECT RetroID from rawPlayers";
    SQL += " WHERE [Value] is null";
    System::Data::OleDb::OleDbCommand^ aCmd = m_DB->CreateCommand();
    aCmd->CommandText = SQL;
    aCmd->CommandTimeout = 0;
    System::Data::OleDb::OleDbDataReader^ rdr = aCmd->ExecuteReader(System::Data::CommandBehavior::SequentialAccess);
    System::Collections::Generic::List<System::String^>^ retval = gcnew System::Collections::Generic::List<System::String^>;
    while (rdr->Read())
    {
        retval->Add(System::Convert::ToString(rdr[0]));
    }
    rdr->Close();
    return retval;
}

System::Collections::Generic::List<System::Int32>^ Retro::RetroDB::AllPlayerIDs(System::Void)
{
    System::String^ SQL = "SELECT ID from rawPlayers";
    SQL += " WHERE [Freq] is null"
           " OR [FreqPitch] is null";
    System::Data::OleDb::OleDbCommand^ aCmd = m_DB->CreateCommand();
    aCmd->CommandText = SQL;
    aCmd->CommandTimeout = 0;
    System::Data::OleDb::OleDbDataReader^ rdr = aCmd->ExecuteReader(System::Data::CommandBehavior::SequentialAccess);
    System::Collections::Generic::List<System::Int32>^ retval = gcnew System::Collections::Generic::List<System::Int32>;
    while (rdr->Read())
    {
        retval->Add(System::Convert::ToInt32(rdr[0]));
    }
    rdr->Close();
    return retval;
}

System::Void Retro::RetroDB::AddIndivResult (System::String^ id, System::Double v)
{
    if (System::Double::IsNaN(v)) v=System::Single::MaxValue;
    System::String^ SQL = "UPDATE rawPlayers" + System::Environment::NewLine +
                          " SET [Value]=" + v + System::Environment::NewLine +
                          " WHERE [RetroID]='"+id+"'";
    System::Data::OleDb::OleDbCommand^ aCmd = m_DB->CreateCommand();
    aCmd->CommandText = SQL;
    aCmd->CommandTimeout = 0;
    aCmd->ExecuteNonQuery();
}

System::Void Retro::RetroDB::AddIndivIDResult (System::Int32 id, System::Int32 f, System::Double v)
{
    System::String^ SQL = "UPDATE rawPlayers" + System::Environment::NewLine +
                          " SET [Total]=" + v + System::Environment::NewLine +
                               ",[Freq]=" + f  + System::Environment::NewLine +
                          " WHERE [ID]="+id;
    System::Data::OleDb::OleDbCommand^ aCmd = m_DB->CreateCommand();
    aCmd->CommandText = SQL;
    aCmd->CommandTimeout = 0;
    aCmd->ExecuteNonQuery();
}

System::Void Retro::RetroDB::AddIndivIDPitchResult (System::Int32 id, System::Int32 f, System::Double v)
{
    System::String^ SQL = "UPDATE rawPlayers" + System::Environment::NewLine +
                          " SET [TotalPitch]=" + v + System::Environment::NewLine +
                               ",[FreqPitch]=" + f  + System::Environment::NewLine +
                          " WHERE [ID]="+id;
    System::Data::OleDb::OleDbCommand^ aCmd = m_DB->CreateCommand();
    aCmd->CommandText = SQL;
    aCmd->CommandTimeout = 0;
    aCmd->ExecuteNonQuery();
}