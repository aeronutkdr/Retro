#include "StdAfx.h"
#include "DBDictionary.h"

using namespace Retro;

DBDictionary::DBDictionary(System::Data::OleDb::OleDbConnection^ db,
                           System::String^ tbl,
                           System::Boolean preLoad)
{
    m_DB      = db;
    m_Table   = tbl;
    m_Preload = preLoad;

    if (m_Preload)
    {
        System::Diagnostics::Trace::Assert
            (m_DB->State == System::Data::ConnectionState::Open);
        System::Data::OleDb::OleDbCommand^ aCmd = m_DB->CreateCommand();
        aCmd->CommandText = "SELECT"
                            " COUNT ([" + m_Table + "].[ID])"
                            " FROM [" + m_Table + "]";
        System::Diagnostics::Debug::Print (aCmd->CommandText);
        System::Int32^ count = (System::Int32^) aCmd->ExecuteScalar();
        m_Dict = gcnew System::Collections::Generic::Dictionary<System::String^, System::Int32>(*count);

        aCmd->CommandText = "SELECT"
                            " [" + m_Table + "].[ID]"
                            ",[" + m_Table + "].[Name]"
                            " FROM [" + m_Table + "]";
        System::Diagnostics::Debug::Print (aCmd->CommandText);
        System::Data::OleDb::OleDbDataReader^ aRdr = aCmd->ExecuteReader();

        while (aRdr->Read())
            m_Dict->Add (aRdr->GetString(1), aRdr->GetInt32(0));
        aRdr->Close();
    }
    else
    {
        m_Dict = gcnew System::Collections::Generic::Dictionary<System::String^, System::Int32>();
    }
}

System::Boolean DBDictionary::Add (System::String^ name,
                                   System::Int32% ID)
{
    //return false if already exists
    //       true  if added
    System::Boolean retval = false;
    if (System::String::IsNullOrEmpty(name))
    {
        ID = 0;
        return false;
    }
    if (!m_Dict->TryGetValue (name, ID))
    {
        System::Diagnostics::Trace::Assert
                    (m_DB->State == System::Data::ConnectionState::Open);
        System::Data::OleDb::OleDbCommand^ aCmd = m_DB->CreateCommand();
        aCmd->CommandText = "SELECT"
                            " [" + m_Table + "].[ID]"
                            " FROM [" + m_Table + "]"
                            " WHERE"
                            " [" + m_Table + "].[Name]='" + name + "'";
        //System::Diagnostics::Debug::Print (aCmd->CommandText);
        if (!m_Preload)
        {
            // check database
            System::Int32^ v = (System::Int32^) aCmd->ExecuteScalar();
            ID = v==nullptr?0:*v;
        }
        if (ID == 0)
        {
            retval = true;
            System::Data::OleDb::OleDbCommand^ aInsCmd = m_DB->CreateCommand();
            aInsCmd->CommandText = "INSERT INTO"
                                   " [" + m_Table + "] ("
                                   "[Name]"
                                   ")"
                                   " VALUES"
                                   " ('" + name + "')";
            //System::Diagnostics::Debug::Print (aInsCmd->CommandText);
            System::Diagnostics::Trace::Assert (aInsCmd->ExecuteNonQuery() == 1);
            System::Int32^ v = (System::Int32^) aCmd->ExecuteScalar();
            System::Diagnostics::Trace::Assert (v != nullptr);
            ID = *v;
        }
        m_Dict->Add (name, ID);
    }
    return retval;
}

System::Int32 DBDictionary::Add (System::String^ name)
{
    System::Int32 retval;
    Add (name, retval);
    return retval;
}

System::Void DBDictionary::Clear (System::Void)
{
    if (!m_Preload) m_Dict->Clear();
}
