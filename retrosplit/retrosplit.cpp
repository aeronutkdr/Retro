// retrosplit.cpp : main project file.

#include "stdafx.h"

using namespace System;

int main(array<System::String ^> ^args)
{
    for each (System::String^ s in args)
    {
        System::IO::TextReader^ rdr = gcnew System::IO::StreamReader(s);
        System::Int32 count = 0;
        System::IO::TextWriter^ wtr = gcnew System::IO::StreamWriter(s+(count>>18)+".out");
        System::String^ line;
        while (line = rdr->ReadLine())
        {
            wtr->WriteLine(line);
            count++;
            if ((count & 0x3FFFF) == 0)
            {
                wtr->Close();
                wtr = gcnew System::IO::StreamWriter(s+(count>>18)+".out");
            }
        }
        rdr->Close();
        wtr->Close();
    }
    return 0;
}
