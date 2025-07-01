#include "StateSolve.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

static void SS_Fill(double* m, struct StateType* s)
{
    for (struct ToType* t = s->to0; t; t=t->nextTo)
    {
        m[t->st->order] = t->freq;
    }
}
unsigned int SS_Solve (struct StateType* root,
                       double* matrix,
                       unsigned int n)
{
    assert (State_Num(root) == n);
    for (int i=0; i<n; i++)
    {
        double* p = matrix + (i * (n+1));
        memset (p, 0, (n+1)*sizeof (double));
        SS_Fill(p, root);
        if (p[root->order] != 0.)
        {
            fprintf (stderr,
                     "matrix[%d, %d] = %f\n",
                     i, root->order, p[root->order]);
        }
        p[root->order] -= (root->freq - root->endinning);
        if (p[root->order] == 0) p[root->order]--;
        if (State_TransNum(root)) p[n] = -(double)State_Total(root);
        root = root->next;
    }
    return 0;
}

void SS_DumpMatrix (struct StateType* root,
                    double* matrix,
                    unsigned int n)
{
    fprintf (stdout, ",");
    for (struct StateType* s = root; s; s = s->next)
    {
        fprintf (stdout, "%04X,", s->value);
    }
    fprintf (stdout, "\n");
    for (unsigned int i=0; i<n; i++, root = root->next)
    {
        double* p = matrix + i * (n+1);
        fprintf (stdout, "%04X,", root->value);
        for (unsigned int j=0; j<n+1; j++)
        {
            if ((int) p[j]) fprintf (stdout, "%d,", (int) p[j]);
            else            fprintf (stdout, ",");
        }
        fprintf (stdout, "\n");
    }
}

void SS_Final (struct StateType* root,
               double* matrix,
               unsigned int n,
               unsigned int flag)
{
    if (flag == 2)
    {
        fprintf (stdout, "Infinite Solutions Exists\n");
    }
    else if (flag == 3)
    {
        fprintf (stdout, "No Solution Exists\n");
    }
    else
    {
        for (unsigned int i=0; i<n; i++, root = root->next)
        {
            double* p = matrix + i * (n+1);
            assert (p[i] != 0.);
            fprintf (stdout, "%04X,%d,%f\n",
                     root->value,
                     root->freq,
                     p[n]/p[i]);
        }
    }
}