#ifndef _GJ_H_
#define _GJ_H_

void    PrintMatrix     (double *a, int n);
int     PerformOperation(double *a, int n);
void    PrintResult     (double *a, int n, int flag);
double* BuildResults    (double *a, int n, int flag);
int     CheckConsistency(double *a, int n, int flag);

#endif /* _GJ_H_ */