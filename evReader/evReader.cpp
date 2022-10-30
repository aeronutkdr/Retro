// evReader.cpp : main project file.

#include "stdafx.h"

using namespace System;

System::Collections::Generic::List<System::Byte>^ ProcessPitches (System::String^ s);
System::Byte ProcessPitch (System::Char c);

int main(array<System::String ^> ^args)
{
    System::IO::TextReader^ rdr = gcnew System::IO::StreamReader(L"C:\\Users\\Kevin\\Documents\\retrosheet\\data\\ev_ files\\2000ANA.EVA");
    //rdr = gcnew System::IO::StreamReader(s);

    return 0;
}

System::Collections::Generic::List<System::Byte>^ ProcessPitches (System::String^ s)
{
    System::Collections::Generic::List<System::Byte>^ retval = gcnew System::Collections::Generic::List<System::Byte>();
    System::Byte count = 0;
    for each (System::Char c in s)
    {
        count += ProcessPitch(c);
        retval->Add(count);
    }
    return retval;
}


#define STRIKE (1)
#define BALL (4)
System::Byte ProcessPitch (System::Char c)
{
    System::Byte retval = 0;
    switch (c)
    {
    case 'B'://  ball
    case 'I'://  intentional ball
    case 'V'://  called ball because pitcher went to his mouth
    case 'P'://  pitchout
        retval = BALL;
    break;

    case 'C'://  called strike
    case 'S'://  swinging strike
    case 'F'://  foul
    case 'L'://  foul bunt
    case 'O'://  foul tip on bunt
    case 'R'://  foul ball on pitchout
    case 'T'://  foul tip
    case 'Q'://  swinging on pitchout
    case 'M'://  missed bunt attempt
        retval = STRIKE;
    break;

    case '*'://  indicates the following pitch was blocked by the catcher
    case '1'://  pickoff throw to first
    case '2'://  pickoff throw to second
    case '3'://  pickoff throw to third
    case '7'://  pickoff throw to left field?
    case '+'://  following pickoff throw by the catcher
    case '>'://  Indicates a runner going on the pitch
    case '.'://  marker for play not involving the batter
    case 'H'://  hit batter
    case 'N'://  no pitch (on balks and interference calls)
    case 'X'://  ball put into play by batter
    case 'Y'://  ball put into play on pitchout
    case 'K'://  strike (unknown type)
    case 'D'://  not described by website but used in 2015ATL.EVN "BFBFDX"
    break;

    case 'U'://  unknown or missed pitch
    default:
        System::Diagnostics::Debug::Assert (false);
    break;
    }
    return retval;
}