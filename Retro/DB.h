#pragma once
#include "FileEvent.h"

namespace Retro
{

    public ref class DB
    {
    protected:
        ref class GameData
        {
        public:
            System::Int32    m_ID;
            System::Int32    m_VisitTeamID;
            System::Int32    m_SequenceNumber;
            System::DateTime m_Date;
        };

        ref class EventData
        {
        public:
            System::Int32   m_GameID;
            System::Int32   m_Inning;
            System::Boolean m_HomeTeamBatting;
            System::Int32   m_Base0ID;
            System::Int32   m_Base1ID;
            System::Int32   m_Base2ID;
            System::Int32   m_Base3ID;
            System::Byte    m_Start;
            System::Byte    m_End;
        };
    protected:
        System::Collections::Generic::Dictionary<System::String^, System::Int32>^ m_Files;
        System::Collections::Generic::Dictionary<System::String^, System::Int32>^ m_Leagues;
        System::Collections::Generic::Dictionary<System::String^, System::Int32>^ m_Players;
        System::Collections::Generic::Dictionary<System::String^, System::Int32>^ m_Teams;
        System::Collections::Generic::Dictionary<System::String^, GameData^>^     m_Games;
        System::Collections::Generic::Dictionary<System::String^, EventData^>^    m_Events;
        System::Data::OleDb::OleDbConnection^ m_DB;
        System::Int32 AddFile (System::String^ file);
        System::Int32 AddGame (System::String^ game,
                               System::Int32   fileID,
                               System::String^ visitTeam);
        System::Int32 AddSimple (System::String^ table,
                                 System::String^ name);
    public:
        DB(void);
        DB(System::String^ connection);
        System::Boolean Add (System::String^ file, FileEvent^ evt);
        System::Boolean Add (FileEvent^ evt);
        System::Void Close (System::Void);
        System::Void Clean (System::Void);
        System::Boolean FileExists (System::String^ fname);
        System::Void Erase (System::Void);
    };
}