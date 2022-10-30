// retrocheck.cpp : main project file.

#include "stdafx.h"

using namespace System;

int main(array<System::String ^> ^args)
{
    for each (System::String^ s in args)
    {
        System::IO::TextReader^ rdr = gcnew System::IO::StreamReader(s);
        //System::IO::TextWriter^ wtr = gcnew System::IO::StreamWriter(s+".out");
        System::String^ line;
        //System::String^ lineOld;
        //System::Boolean write = false;
        while (line = rdr->ReadLine())
        {
            if (System::Text::RegularExpressions::Regex::IsMatch(line, "[^,]{45,}"))
            {
                System::Console::WriteLine(line);
            }
            //if (write) wtr->WriteLine(lineOld);
            //write = true;
            //lineOld = line;
        }
        //System::Console::WriteLine (lineOld);
        rdr->Close();
        //wtr->Close();
    }
    return 0;
}