#pragma once

namespace Retro
{
    public ref class DBDictionary
    {
    protected:
        System::Data::OleDb::OleDbConnection     ^m_DB;
        System::Collections::Generic::Dictionary
            <System::String^, System::Int32>     ^m_Dict;
        System::Boolean                           m_Preload;
    public:
        System::String                           ^m_Table;
        DBDictionary(System::Data::OleDb::OleDbConnection^ db,
                     System::String^ tbl,
                     System::Boolean preLoad);
        System::Int32 Add (System::String^ name);
        System::Boolean Add (System::String^ name,
                             System::Int32%  ID);
        System::Void Clear (System::Void);
    };
}
