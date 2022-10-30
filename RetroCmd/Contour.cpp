#include "stdafx.h"
#include "Contour.h"

#define xsect(p1,p2) (h[p2]*xh[p1]-h[p1]*xh[p2])/(h[p2]-h[p1])
#define ysect(p1,p2) (h[p2]*yh[p1]-h[p1]*yh[p2])/(h[p2]-h[p1])
#define min(x,y) ((x)<(y)?(x):(y))
#define max(x,y) ((x)>(y)?(x):(y))

System::Collections::Generic::List<array<System::Double, 1>^>^
              conrec(array<System::Double, 2>^ d,
                     array<System::Double, 1>^ x,
                     array<System::Double, 1>^ y,
                     array<System::Double, 1>^ z)
{
    System::Collections::Generic::List<array<System::Double, 1>^>^ retval =
        gcnew System::Collections::Generic::List<array<System::Double, 1>^>;
    const System::Int32 ilb = 0;
    const System::Int32 iub = d->GetLength(0)-1;
    const System::Int32 jlb = 0;
    const System::Int32 jub = d->GetLength(1)-1;
    const System::Int32 nc  = z->GetLength(0);
    System::Diagnostics::Trace::Assert(d->GetLength(0) == x->GetLength(0));
    System::Diagnostics::Trace::Assert(d->GetLength(1) == y->GetLength(0));

    System::Int32 m1,m2,m3,case_value;
    System::Double dmin,dmax,x1,x2,y1,y2;
    register System::Int32 i,j,k,m;
    System::Double h[5];
    System::Int32 sh[5];
    System::Double xh[5],yh[5];
    //===========================================================================
    // The indexing of im and jm should be noted as it has to start from zero
    // unlike the fortran counter part
    //===========================================================================
    System::Int32 im[4] = {0,1,1,0},jm[4]={0,0,1,1};
    //===========================================================================
    // Note that castab is arranged differently from the FORTRAN code because
    // Fortran and C/C++ arrays are transposed of each other, in this case
    // it is more tricky as castab is in 3 dimension
    //===========================================================================
    System::Int32 castab[3][3][3] =
    {
      {
        {0,0,8},{0,2,5},{7,6,9}
      },
      {
        {0,3,4},{1,3,1},{4,3,0}
      },
      {
        {9,6,7},{5,2,0},{8,0,0}
      }
    };
    for (j=(jub-1);j>=jlb;j--) {
        for (i=ilb;i<=iub-1;i++) {
            System::Double temp1,temp2;
            temp1 = min((d[i,j]),(d[i,j+1]));
            temp2 = min((d[i+1,j]),(d[i+1,j+1]));
            dmin = min(temp1,temp2);
            temp1 = max((d[i,j]),(d[i,j+1]));
            temp2 = max((d[i+1,j]),(d[i+1,j+1]));
            dmax = max(temp1,temp2);
            if (dmax>=z[0]&&dmin<=z[nc-1]) {
                for (k=0;k<nc;k++) {
                    if (z[k]>=dmin&&z[k]<=dmax) {
                        for (m=4;m>=0;m--) {
                            if (m>0) {
                                //=============================================================
                                // The indexing of im and jm should be noted as it has to
                                // start from zero
                                //=============================================================
                                h[m] = d[i+im[m-1],j+jm[m-1]]-z[k];
                                xh[m] = x[i+im[m-1]];
                                yh[m] = y[j+jm[m-1]];
                            } else {
                                h[0] = 0.25*(h[1]+h[2]+h[3]+h[4]);
                                xh[0]=0.5*(x[i]+x[i+1]);
                                yh[0]=0.5*(y[j]+y[j+1]);
                            }
                            if (h[m]>0.0) {
                                sh[m] = 1;
                            } else if (h[m]<0.0) {
                                sh[m] = -1;
                            } else
                                sh[m] = 0;
                        }
                        //=================================================================
                        //
                        // Note: at this stage the relative heights of the corners and the
                        // centre are in the h array, and the corresponding coordinates are
                        // in the xh and yh arrays. The centre of the box is indexed by 0
                        // and the 4 corners by 1 to 4 as shown below.
                        // Each triangle is then indexed by the parameter m, and the 3
                        // vertices of each triangle are indexed by parameters m1,m2,and
                        // m3.
                        // It is assumed that the centre of the box is always vertex 2
                        // though this is important only when all 3 vertices lie exactly on
                        // the same contour level, in which case only the side of the box
                        // is drawn.
                        //
                        //
                        //      vertex 4 +-------------------+ vertex 3
                        //               | \               / |
                        //               |   \    m-3    /   |
                        //               |     \       /     |
                        //               |       \   /       |
                        //               |  m=2    X   m=2   |       the centre is vertex 0
                        //               |       /   \       |
                        //               |     /       \     |
                        //               |   /    m=1    \   |
                        //               | /               \ |
                        //      vertex 1 +-------------------+ vertex 2
                        //
                        //
                        //
                        //               Scan each triangle in the box
                        //
                        //=================================================================
                        for (m=1;m<=4;m++) {
                            m1 = m;
                            m2 = 0;
                            if (m!=4)
                                m3 = m+1;
                            else
                                m3 = 1;
                            case_value = castab[sh[m1]+1][sh[m2]+1][sh[m3]+1];
                            if (case_value!=0) {
                                switch (case_value) {
                                //===========================================================
                                //     Case 1 - Line between vertices 1 and 2
                                //===========================================================
                                case 1:
                                    x1=xh[m1];
                                    y1=yh[m1];
                                    x2=xh[m2];
                                    y2=yh[m2];
                                break;
                                //===========================================================
                                //     Case 2 - Line between vertices 2 and 3
                                //===========================================================
                                case 2:
                                    x1=xh[m2];
                                    y1=yh[m2];
                                    x2=xh[m3];
                                    y2=yh[m3];
                                break;
                                //===========================================================
                                //     Case 3 - Line between vertices 3 and 1
                                //===========================================================
                                case 3:
                                    x1=xh[m3];
                                    y1=yh[m3];
                                    x2=xh[m1];
                                    y2=yh[m1];
                                break;
                                //===========================================================
                                //     Case 4 - Line between vertex 1 and side 2-3
                                //===========================================================
                                case 4:
                                    x1=xh[m1];
                                    y1=yh[m1];
                                    x2=xsect(m2,m3);
                                    y2=ysect(m2,m3);
                                break;
                                //===========================================================
                                //     Case 5 - Line between vertex 2 and side 3-1
                                //===========================================================
                                case 5:
                                    x1=xh[m2];
                                    y1=yh[m2];
                                    x2=xsect(m3,m1);
                                    y2=ysect(m3,m1);
                                break;
                                //===========================================================
                                //     Case 6 - Line between vertex 3 and side 1-2
                                //===========================================================
                                case 6:
                                    x1=xh[m3];
                                    y1=yh[m3];
                                    x2=xsect(m1,m2);
                                    y2=ysect(m1,m2);
                                break;
                                //===========================================================
                                //     Case 7 - Line between sides 1-2 and 2-3
                                //==========================================================
                                case 7:
                                    x1=xsect(m1,m2);
                                    y1=ysect(m1,m2);
                                    x2=xsect(m2,m3);
                                    y2=ysect(m2,m3);
                                break;
                                //===========================================================
                                //     Case 8 - Line between sides 2-3 and 3-1
                                //===========================================================
                                case 8:
                                    x1=xsect(m2,m3);
                                    y1=ysect(m2,m3);
                                    x2=xsect(m3,m1);
                                    y2=ysect(m3,m1);
                                break;
                                //===========================================================
                                //     Case 9 - Line between sides 3-1 and 1-2
                                //===========================================================
                                case 9:
                                    x1=xsect(m3,m1);
                                    y1=ysect(m3,m1);
                                    x2=xsect(m1,m2);
                                    y2=ysect(m1,m2);
                                break;
                                default:
                                break;
                                }
                                //=============================================================
                                // Put your processing code here and comment out the printf
                                //=============================================================
                                retval->Add (gcnew array<System::Double, 1>
                                                {x1, y1, x2, y2, z[k]});
                                /*
                                System::Console::WriteLine
                                 (System::String::Format
                                   ("{0:F5}\t{1:F5}\t{2:F5}\t{3:F5}\t{4:F5}",
                                    x1,y1,x2,y2,z[k]));
                                */
                            }
                        }
                    }
                }
            }
        }
    }
    return retval;
}

System::Void reorder (System::Collections::Generic::List<array<System::Double, 1>^>^ contour)
{
    /* sort x0 in array*/
    for each (array<System::Double, 1>^ a in contour)
    {
        if (a[0] > a[2])
        {
            System::Double tmp  = a[0];
                           a[0] = a[2];
                           a[2] = tmp;
                           tmp  = a[1];
                           a[1] = a[3];
                           a[3] = tmp;
        }
    }

    /* sort z (if z is equal, sort x0 among array) */
    for (System::Int32 i=0; i<contour->Count-1; i++)
    {
        System::Int32 low_j = i;
        for (System::Int32 j=i+1; j<contour->Count; j++)
        {
            if ((contour[j][4] < contour[low_j][4]) ||
                (contour[j][4] == contour[low_j][4] &&
                 contour[j][0] < contour[low_j][0]))
            {
                low_j = j;
            }
        }
        if (low_j != i)
        {
            array<System::Double, 1>^ tmp = gcnew array<System::Double, 1>(contour[i]->GetLength(0));
            System::Array::Copy (contour[low_j] ,
                                 tmp,
                                 contour[i]->GetLength(0));
            System::Array::Copy (contour[i],
                                 contour[low_j],
                                 contour[i]->GetLength(0));
            System::Array::Copy (tmp,
                                 contour[i],
                                 contour[i]->GetLength(0));
        }
    }
}
