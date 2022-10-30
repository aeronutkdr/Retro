// AppendRoster.cpp : main project file.

#include "stdafx.h"

using namespace System;

int main(array<System::String ^> ^args)
{
    array<System::String^>^ files = System::IO::Directory::GetFiles("C:\\Users\\Kevin\\Documents\\retrosheet\\data\\roster files\\years", "*.ros");
    for each (System::String^ file in files)
    {
        System::IO::TextReader^ rdr = gcnew System::IO::StreamReader(file);
        System::IO::TextWriter^ wtr = gcnew System::IO::StreamWriter(file + ".out");
        System::String^ line;
        System::String^ year = file->Substring(60,4);
        while (((line = rdr->ReadLine()) != nullptr) &&
                (line->Length > 1))
        {
            wtr->WriteLine(line+","+year);
        }
        rdr->Close();
        wtr->Close();
    }
    return 0;
}
