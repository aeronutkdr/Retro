// GJSolve.cpp : main project file.

#include "stdafx.h"
#include "..\\RetroCmd\\MathUtils.h"

using namespace System;

array<System::Int32>^   MaskRuns(array<System::Int32>^ R, System::Int32 mask);
array<System::Int32,2>^ MaskMatx(array<System::Int32, 2>^ M, System::Int32 mask);

System::String^ Dump (array<System::Double>^ R, array<System::Double, 2>^ M);

/* OO321HCCCC */
#define MASK (0x0F0)
//#define MASK (0x3E0)
int main(array<System::String ^> ^args)
{
    System::String^ line;
    System::IO::TextReader^ rdr = gcnew System::IO::StreamReader(args[0]);
    rdr->ReadLine();
    line = rdr->ReadLine();
    array<System::String^>^ arr = line->Split(' ');
    array<System::Int32>^ Runs = gcnew array<System::Int32>(1024);
    for (System::Int32 i=0; i<1024; i++)
        Runs[i] = System::Convert::ToInt32(arr[i]);
    array<System::Int32,2>^ Matrix = gcnew array<System::Int32,2>(1024,1024);
    for (System::Int32 i=0; i<1024; i++)
    {
        line = rdr->ReadLine();
        arr = line->Split(' ');
        for (System::Int32 j=0; j<1024; j++)
            Matrix[j,i] = System::Convert::ToInt32(arr[j]);
    }
    rdr->Close();
    array<System::Int32>^ MRuns = MaskRuns(Runs,MASK);
    array<System::Int32,2>^ MMatx = MaskMatx(Matrix, MASK);
    array<System::Boolean>^ Zero = gcnew array<System::Boolean>(MRuns->Length);
    System::Int32 Num = 0;
    for (System::Int32 i=0; i<MRuns->Length; i++)
    {
        Zero[i] = true;
        for (System::Int32 j=0; Zero[i] && j<MRuns->Length; j++)
        {
            Zero[i] &= MMatx[i,j] == 0;
        }
        if (!Zero[i])
        {
            Num++;
        }
    }
    System::Int32 ii = 0;
    array<System::Double,2>^ DMatrix = gcnew array<System::Double,2>(Num,Num);
    array<System::Double>^ DRuns = gcnew array<System::Double>(Num);
    for (System::Int32 i=0; i<MRuns->Length; i++)
    {
        if (Zero[i]) continue;
        DRuns[ii] = System::Convert::ToDouble(MRuns[i]);
        System::Int32 jj = 0;
        for (System::Int32 j=0; j<MRuns->Length; j++)
        {
            if (Zero[j]) continue;
            DMatrix[ii,jj] = System::Convert::ToDouble(MMatx[i,j]);
            jj++;
        }
        ii++;
    }
    //https://planetcalc.com/3571/
    System::Console::WriteLine(Dump(DRuns,DMatrix));
    GJ_Solve(DMatrix,DRuns);
    System::Console::WriteLine(Dump(DRuns,DMatrix));
    //for (System::Int32 i=0, ii=0; i<1024; i++)
    //{
    //    if (Zero[i]) continue;
    //    System::Console::WriteLine(i + " = " + DRuns[ii]);
    //    ii++;
    //}
    return 0;
}


array<System::Int32>^   MaskRuns(array<System::Int32>^ R, System::Int32 mask)
{
    System::Int32 cnt = 0;
    System::Int32 m = mask;
    while (m)
    {
        cnt++;
        m>>=1;
    }
    cnt = 10;
    array<System::Int32>^ retval = gcnew array<System::Int32>(1<<cnt);
    for (System::Int32 i=0; i<retval->Length; i++) retval[i] = 0;
    for (System::Int32 i=0; i<R->Length; i++)
    {
        retval[i&mask] += R[i];
    }
    return retval;
}
array<System::Int32,2>^ MaskMatx(array<System::Int32, 2>^ M, System::Int32 mask)
{
    System::Int32 cnt = 0;
    System::Int32 m = mask;
    while (m)
    {
        cnt++;
        m>>=1;
    }
    cnt = 10;
    array<System::Int32,2>^ retval = gcnew array<System::Int32,2>(1<<cnt,1<<cnt);
    for (System::Int32 i=0; i<retval->GetLength(0); i++)
        for (System::Int32 j=0; j<retval->GetLength(0); j++)
            retval[i,j] = 0;
    for (System::Int32 i=0; i<M->GetLength(0); i++)
        for (System::Int32 j=0; j<M->GetLength(0); j++)
        {
            retval[i&mask,j&mask] += M[i,j];
        }
    return retval;
}

System::String^ Dump (array<System::Double>^ R, array<System::Double, 2>^ M)
{
    System::String^ retval;
    for (System::Int32 i=0; i<R->Length; i++)
    {
        for (System::Int32 j=0; j<R->Length; j++)
            retval += M[i,j] + "\t";
        retval += R[i] + System::Environment::NewLine;
    }
    return retval;
}