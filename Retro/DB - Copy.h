#pragma once
#include "FileEvent.h"

namespace Retro
{
    public ref class DB
    {
    protected:
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
        System::Void Close (System::Void);
        System::Void Clean (System::Void);
    };
}