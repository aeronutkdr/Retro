#ifndef _CONTOUR_H_
#define _CONTOUR_H_

// d ! matrix of data to contour
// x ! data matrix column coordinates
// y ! data matrix row coordinates
// z ! contour levels in increasing order
System::Collections::Generic::List<array<System::Double, 1>^>^
                          conrec(array<System::Double, 2>^ d,
                                 array<System::Double, 1>^ x,
                                 array<System::Double, 1>^ y,
                                 array<System::Double, 1>^ z);

System::Void reorder (System::Collections::Generic::List<array<System::Double, 1>^>^ contour);

#endif /* _CONTOUR_H_ */