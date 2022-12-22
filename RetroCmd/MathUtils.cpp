#include "stdafx.h"
#include "MathUtils.h"
System::Int32 GCF (System::Int32 x, System::Int32 y)
{
    if (x == 0 || y == 0) return 1;
    System::Int32 sgn_x = System::Math::Sign(x);
    System::Int32 sgn_y = System::Math::Sign(y);
    x *= sgn_x;
    y *= sgn_y;
    if (x < y)
    {
        System::Int32 tmp = x;
                        x = y;
                        y = tmp;
    }
    System::Int32 rem, res = System::Math::DivRem(x, y, rem);;
    /* negate the factor to make y positive (assume x=n, y=d) */
    if (rem == 0) return y           *sgn_y;
    else          return GCF (y, rem)*sgn_y;
}

System::Int32 GCF (array<System::Int32, 1>^ v)
{
    array<System::Int32>^ gcfs = gcnew array<System::Int32>(v->GetLength(0)-1);
    for (System::Int32 i=0; i<gcfs->GetLength(0); i++)
        gcfs[i] = GCF (v[i], v[i+1]);
    if (gcfs->GetLength(0) == 1) return gcfs[0];
    else                         return GCF(gcfs);
}

System::Void Fraction::Reduce (System::Void)
{
    System::Int32 gcf = GCF(n, d);
    n /= gcf;
    d /= gcf;
}
Fraction::Fraction(System::Int32 x,
                   System::Int32 y)
{
    n = x;
    d = y;
    System::Diagnostics::Trace::Assert (y!=0);
    Reduce();
}
/*
System::Void GJ_Solve (array<Fraction, 2>^ a,
                       array<Fraction, 1>^ b)
{
    System::Diagnostics::Trace::Assert
                                   (a->GetLength(0) == a->GetLength(1) &&
                                    a->GetLength(0) == b->GetLength(0));
    System::Int32 n = a->GetLength(0);

    for (System::Int32 i=0; i<n; i++)
    {
        // need to create a copy - loop modifies pivot to 1/1
        Fraction pivot = a[i,i];
        for (System::Int32 j=i; j<n; j++)
        {
            a[i,j] /= pivot;
        }
        b[i] /= pivot;
        for (System::Int32 ii=i+1; ii<n; ii++)
        {
            Fraction factor   = a[ii,i] / a[i,i];
            for (System::Int32 j=i; j<n; j++)
            {
                Fraction subtract = a[i,j] * factor;
                a[ii,j] -= subtract;
            }
            Fraction subtract = b[i] * factor;
            b[ii] -= subtract;
        }
    }
}
*/
template <class NumberType>
System::Void GJ_Solve (array<NumberType, 2>^ a,
                       array<NumberType, 1>^ b)
{
#if 0
    System::Diagnostics::Trace::Assert
                                   (a->GetLength(0) == a->GetLength(1) &&
                                    a->GetLength(0) == b->GetLength(0));
    System::Int32 n = a->GetLength(0);

    for (System::Int32 i=0; i<n-1; i++)
    {
        NumberType mult = a[i,i];
        for (System::Int32 ii=i+1; ii<n; ii++)
        {
            NumberType div = a[ii,ii-1];
            if (div != 0.)
            {
                for (System::Int32 jj=i; jj<n; jj++)
                {
                    System::Double add = a[i,jj];
                    a[ii,jj] = -a[ii,jj]*mult/div+add;
                }
            }
            b[ii] = -b[ii]*mult/div+b[i];
        }
        break;
    }
#else
        System::Diagnostics::Trace::Assert
                                   (a->GetLength(0) == a->GetLength(1) &&
                                    a->GetLength(0) == b->GetLength(0));
    System::Int32 n = a->GetLength(0);

    for (System::Int32 i=0; i<n; i++)
    {
        // need to create a copy - loop modifies pivot to 1
        NumberType pivot = a[i,i];
        for (System::Int32 j=i; j<n; j++)
        {
            a[i,j] /= pivot;
        }
        b[i] /= pivot;
        for (System::Int32 ii=i+1; ii<n; ii++)
        {
            NumberType factor   = a[ii,i] / a[i,i];
            for (System::Int32 j=i; j<n; j++)
            {
                NumberType subtract = a[i,j] * factor;
                a[ii,j] -= subtract;
            }
            NumberType subtract = b[i] * factor;
            b[ii] -= subtract;
        }
    }
    for (System::Int32 i=a->GetLength(0)-1; i>0; i--)
    {
        for (System::Int32 ii=i-1; ii>=0; ii--)
        {
            NumberType factor = a[ii,i] / a[i,i];
            b[ii] -= b[i] * factor;
            for (System::Int32 j=i; j<n; j++)
            {
                a[ii,j] -= a[i,j] * factor;
            }
        }
    }
#endif
}