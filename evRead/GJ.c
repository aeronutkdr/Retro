#include "GJ.h"
#include <stdio.h>
#include <stdlib.h>
//#include "mystdlib.h"
//https://www.geeksforgeeks.org/program-for-gauss-jordan-elimination-method/

static void swap(double* a, double *b);

// Function to print the matrix
void PrintMatrix(double *a, int n)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= n; j++)
          printf("%2.2f ", a[(n+1)*i+j]);
        printf("\n");
    }
}

int PerformOperation(double *a, int n)
{
    int i, j, k = 0, c, flag = 0;
    // Performing elementary operations
    for (i = 0; i < n; i++)
    {
        //printf ("i=%d\n", i);
        if (a[i*(n+1)+i] == 0)
        {
            c = 1;
            while ((i + c) < n && a[(i + c)*(n+1)+i] == 0)
                c++;
            if ((i + c) == n) {
                flag = 1;
                break;
            }
            for (j = i, k = 0; k <= n; k++)
                swap(&a[j*(n+1)+k], &a[(j+c)*(n+1)+k]);
        }

        for (j = 0; j < n; j++) {

            // Excluding all i == j
            if (i != j) {

                // Converting Matrix to reduced row
                // echelon form(diagonal matrix)
                double pro = a[j*(n+1)+i] / a[i*(n+1)+i];

                for (k = 0; k <= n; k++)
                    a[j*(n+1)+k] = a[j*(n+1)+k] - (a[i*(n+1)+k]) * pro;
            }
        }
    }
    return flag;
}

double* BuildResults(double * a, int n, int flag)
{
    double *retval;
    if (flag == 2)
      return 0;
    else if (flag == 3)
      return 0;

    else {
        retval = malloc(sizeof(double)*n);
        for (int i = 0; i < n; i++)
            retval[i] = a[i*(n+1)+n] / a[i*(n+1)+i];
    }
    return retval;
}

// Function to print the desired result
// if unique solutions exists, otherwise
// prints no solution or infinite solutions
// depending upon the input given.
void PrintResult(double *a, int n, int flag)
{
    printf ("Result is :\n");

    if (flag == 2)
      printf ("Infinite Solutions Exists\n");
    else if (flag == 3)
      printf ("No Solution Exists\n");


    // Printing the solution by dividing constants by
    // their respective diagonal elements
    else {
        for (int i = 0; i < n; i++)
            printf ("%2.2f\n", a[i*(n+1)+n] / a[i*(n+1)+i]);
    }
}

// To check whether infinite solutions exists or no solution exists
int CheckConsistency(double *a, int n, int flag)
{
    int i, j;
    double sum;

    // flag == 2 for infinite solution
    // flag == 3 for No solution
    flag = 3;
    for (i = 0; i < n; i++)
    {
        sum = 0;
        for (j = 0; j < n; j++)
            sum = sum + a[i*(n+1)+j];
        if (sum == a[i*(n+1)+j])
            flag = 2;
    }
    return flag;
}

// function to reduce matrix to reduced row echelon form.
static void swap(double* a, double *b)
{
	double x = *a;
	*a = *b;
	*b = x;
}