// CRCCalc.cpp : main project file.

#include "stdafx.h"

using namespace System;

int main(array<System::String ^> ^args)
{
    array<System::String^>^ files = System::IO::Directory::GetFiles(args[0], "*.out");
    for each (System::String^ f in files)
    {
        System::IO::BinaryReader^ rdr = gcnew System::IO::BinaryReader(System::IO::File::OpenRead(f));
        array<System::Byte>^ bytes = rdr->ReadBytes((System::Int32) rdr->BaseStream->Length);
        System::Int32 CRC = CRC32::CRC32::calc(bytes);
        System::Int32 last = f->LastIndexOf('\\')+1;
        System::Console::WriteLine (f->Substring(last,f->Length-last-4) + "," + CRC);
        rdr->Close();
    }
    return 0;
}
