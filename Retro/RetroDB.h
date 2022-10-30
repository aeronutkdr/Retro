#pragma once
#include "DBDictionary.h"
#include "RetroEvent.h"

namespace Retro
{
    public ref class ResultType
    {
    public:
        System::Int32 m_Start;
        System::Int32 m_End;
        System::Int32 m_Freq;
    };
    public ref class StateFreqType
    {
    public:
        System::Int32 m_State;
        System::Int32 m_Freq;
        System::Int32 m_StartEnd;
    };
    public ref class IndivResultType
    {
    public:
        System::Double residual;
        System::Int32  runs;
        System::Int32  num;
    };
    public ref class RetroDB
    {
    protected:
        System::Data::OleDb::OleDbConnection^ m_DB;
        Retro::DBDictionary^ Files;
        Retro::DBDictionary^ Games;
        Retro::DBDictionary^ Players;
        Retro::DBDictionary^ Teams;
        System::Int32 AddGame (System::String^ name,
                               System::Int32   fID,
                               System::Int32   vID);
    public:
        enum class EntityType
        {
            Files   = 0x01,
            Games   = 0x02,
            Players = 0x04,
            Teams   = 0x08
        };
        RetroDB(System::String^ connection, System::Int32 PreloadMask);
        System::Void Close (System::Void);
        System::Boolean AddEventFile (System::String^ name, System::Int32% fID, System::Int32 crc);
        //System::Boolean AddEvents (System::String^ name);
        System::Void Clear (System::Void);
        System::Collections::Generic::List<ResultType^>^ Query(System::String^ str);
        System::Collections::Generic::List<StateFreqType^>^ QueryFreq(System::String^ str);
        System::Collections::Generic::List<System::String^>^ AllPlayers(System::Void);
        System::Collections::Generic::List<System::Int32>^ AllPlayerIDs(System::Void);
        System::Boolean UpdateFileCRC (System::String^ name, System::Int32 CRC);
        System::Boolean AddRetroEvent (System::Int32 fID, Retro::RetroEvent ^e);
        System::Void AddIndivResult (System::String^ id, System::Double v);
        System::Void AddIndivIDResult (System::Int32 id, System::Int32 f, System::Double v);
        System::Void AddIndivIDPitchResult (System::Int32 id, System::Int32 f, System::Double v);
    };
}