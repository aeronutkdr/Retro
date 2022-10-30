#pragma once

namespace Retro
{
    public ref class StringList
    {
    protected:
        System::Collections::Generic::List<System::String^>^ m_List;
    public:
        StringList            (System::Void                );
        StringList            (System::String^         File);
        System::Int32 Read    (System::IO::TextReader^ rdr );
        System::Int32 Write   (System::IO::TextWriter^ wtr );
        System::Int32 Write   (System::String^         File);
        System::Int32 Add     (System::String^         val );
    };
}