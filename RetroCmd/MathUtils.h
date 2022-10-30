#ifndef _MATHUTILS_H
#define _MATHUTILS_H

public value class Fraction
{
    System::Int32 n;
    System::Int32 d;
    System::Void Reduce   (System::Void);
public:
    Fraction(System::Int32 x, System::Int32 y);
    System::String^ Dump (System::Void) {return n + "/" + d;}
    Fraction operator*   (Fraction x  ) {return Fraction(n * x.n, d * x.d);}
    Fraction operator/   (Fraction x  ) {return Fraction(n * x.d, d * x.n);}
    Fraction operator+   (Fraction x  ) {return Fraction((n * x.d) + (x.n * d), (d * x.d));}
    Fraction operator-   (Fraction x  ) {return Fraction((n * x.d) - (x.n * d), (d * x.d));}
};

System::Int32 GCF (System::Int32 x, System::Int32 y);
System::Int32 GCF (array<System::Int32, 1>^ v);

template <class NumberType>
System::Void GJ_Solve (array<NumberType, 2>^ a,
                       array<NumberType, 1>^ b);

//template System::Void GJ_Solve<Fraction> (array<Fraction, 2>^ a,
//                                          array<Fraction, 1>^ b);
template System::Void GJ_Solve<System::Double> (array<System::Double, 2>^ a,
                                                array<System::Double, 1>^ b);
//template System::Void GJ_Solve<System::Int32> (array<System::Int32, 2>^ a,
//                                               array<System::Int32, 1>^ b);

#endif /* _MATHUTILS_H */