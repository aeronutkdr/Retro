#include "stdafx.h"
#include "StringList.h"

using namespace Retro;

StringList::StringList(System::Void)
{
    m_List = gcnew System::Collections::Generic::List<System::String^>;
}

System::Int32 StringList::Read (System::IO::TextReader^ rdr)
{
    m_List->Clear();
    System::String^ rd;
    while ((rd = rdr->ReadLine()) != nullptr) m_List->Add (rd);
    return m_List->Count;
}

System::Int32 StringList::Write (System::IO::TextWriter^ wtr)
{
    for each (System::String^ str in m_List)  wtr->WriteLine(str);
    return m_List->Count;
}

System::Int32 StringList::Add  (System::String^ val)
{
    System::Int32 idx = m_List->IndexOf(val);
    if (idx == -1)
    {
        if (!System::String::IsNullOrEmpty(val))
        {
            m_List->Add(val);
            idx = m_List->IndexOf(val);
        }
    }
    return idx;
}

StringList::StringList (System::String^ File)
{
    m_List = gcnew System::Collections::Generic::List<System::String^>;
    System::IO::TextReader^ rdr =
        gcnew System::IO::StreamReader
                  (System::IO::File::Open(File,
                                          System::IO::FileMode::OpenOrCreate));
    Read (rdr);
    rdr->Close();
}

System::Int32 StringList::Write (System::String^ File)
{
    System::IO::TextWriter^ wtr =
        gcnew System::IO::StreamWriter
                        (System::IO::File::Open(File,
                                                System::IO::FileMode::Create));
    System::Int32 retval = Write (wtr);
    wtr->Close();
    return retval;
}