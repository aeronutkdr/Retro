#include "retroparse.h"
#include "NodeTree.h"
#include "GJ.h"
#include <stdio.h>
#include "mystdlib.h"
#include <memory.h>

void BuildHTML (char* fname, struct NT_SolveType* sol, int num, unsigned int* agg, unsigned int* sum);
int main(int argc, char* argv[])
{
#if 0
    double a[] = {0., 2., 1., 4.,
                  1., 1., 2., 6.,
                  2., 1., 1., 7.};

    // Order of Matrix(n)
    int flag = 0;

    //PrintMatrix(a, 3);
    // Performing Matrix transformation
    flag = PerformOperation(a, 3);

    if (flag == 1)
        flag = CheckConsistency(a, 3, flag);

    // Printing Final Matrix
    printf ("Final Augmented Matrix is :\n");
    PrintMatrix(a, 3);
    printf ("\n");

    // Printing Solutions(if exist)
    PrintResult(a, 3, flag);
#elif 1
    int arglen = strlen(argv[1]);
    char* fname = malloc(sizeof(char)*(arglen+18));
    memcpy(fname, "test\\script\\", 12);
    memcpy(fname+12, argv[1], arglen);
    memcpy(fname+12+arglen, ".txt\x00", 5);
    FILE* fp = fopen(fname, "r");
    RP_Parse(fp);
    fclose(fp);
    NT_Dump();
    int num;
    int* freqs = NT_CalcFreqs(&num);
    for (int i=0; i<num; i++)
    {
        if (freqs[i] != freqs[i+num])
            printf ("%04X:\t%d\t%d\n", i, freqs[i], freqs[i+num]);
    }
    memcpy(fname, "test\\actual\\", 12);
    memcpy(fname+12+arglen, ".bin\x00", 5);
    NT_DumpBin(fname);
    struct NT_SolveType* sol = NT_Solve(&num);
    for (int i=0; i<num; i++)
    {
        printf ("%04X: %2.2f\n", sol[i].state, sol[i].value);
    }
    unsigned int agg[0x40*0x40] = {0};
    unsigned int sum[0x40] = {0};
    for (int i=0; i<0x40; i++)
    {
        sum[i] = NT_Aggregate (i, agg+(0x40*i));
        for (int j=0; j<0x40; j++)
        {
            if (agg[0x40*i+j]) printf ("%04X -> %04X:\t%1.3f\n", (i<<8), (j<<8), (double)agg[0x40*i+j]/(double)sum[i]);
        }
    }
    memcpy(fname, "test\\actual\\", 12);
    memcpy(fname+12+arglen, ".html\x00", 6);
    BuildHTML(fname, sol, num, agg, sum);
    free(sol);
    NT_Drop();
    free(fname);
#elif 1
    char line[501];
    FILE* fp = fopen(argv[1], "r");
    while (fgets(line, 500, fp))
    {
        unsigned short *c = RP_ParseLine(line);
        if (c)
        {
            printf ("%2d: ", c[0]);
            for (int i=1; i<c[0]+1; i++)
            {
                printf ("%04X ", c[i]);
            }
            printf ("\n");
            free(c);
        }
    }
    fclose(fp);
#else
    int len = strlen(argv[1]);
    char* p = malloc (len+3);
    memcpy(p+1, argv[1], len);
    p[0] = '\"';
    p[len+1] = '\"';
    p[len+2] = 0;
    int count0;
    sscanf(argv[2], "%x", &count0);
    RP_ParsePitches(p, count0 & 0xFF);
    printf ("%d", p[0]);
    for (int i=1; i<=len; i++)
    {
        printf(" - %02X", (unsigned char)p[i]);
    }
    printf("\n");
    free(p);
#endif
    return 0;
}

int RunValue(unsigned short st)
{
	return ((st>>(0+8))&1) +
	       ((st>>(1+8))&1) +
	       ((st>>(2+8))&1) +
	       ((st>>(3+8))&1) +
	       ((st>>(4+8))&3);
}

#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))

void BuildHTML (char* fname, struct NT_SolveType* sol, int num, unsigned int* agg, unsigned int* sum)
{
    double val[0x40];
    FILE* fp = fopen(fname, "w");
    fprintf (fp, "<!DOCTYPE html>\n");
    //fprintf (fp, "<!--https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D-->\n");
    //fprintf (fp, "<!--https://www.w3schools.com/graphics/tryit.asp?filename=trycanvas_draw-->\n");
    //const int pitch_x = 25;
    const int pitch_x = 3;
    const int base_x = pitch_x * 12;
    const int out_x = base_x * 16;
    const int val_y = 500;
    const double max_y = 3.2;
    const double max_x = 3.9;
    fprintf (fp, "<html>\n");
    fprintf (fp, "<body>\n");
    fprintf (fp, "<h1>%s</h1>\n", fname);
    fprintf (fp, "<canvas id=\"myCanvas\" width=\"%d\" height=\"%d\" style=\"border:1px solid grey;\"></canvas>\n", (int) (out_x*max_x),(int) (max_y*val_y));
    fprintf (fp, "<script>\n");
    fprintf (fp, "const canvas = document.getElementById(\"myCanvas\");\n");
    fprintf (fp, "const ctx = canvas.getContext(\"2d\");\n");
    //fprintf (fp, "ctx.font = \"50px serif\";\n");
    for (int i=0; i<num; i++)
    {
        if ((sol[i].state & 0xFF) != 0) continue;
        val[sol[i].state>>8] = sol[i].value;
        int yi = (max_y - sol[i].value) * val_y;
        int out = (sol[i].state >> 12) & 0x03;
        int bases = (sol[i].state >> 8) & 0x0F;
        int balls = (sol[i].state >> 5) & 0x07;
        int strikes = (sol[i].state) & 0x1F;
        int xi = out * out_x + bases * base_x + (balls + strikes)*pitch_x;
        fprintf (fp, "ctx.fillText(\"%04X\", %d, %d);\n", sol[i].state, xi, yi);
    }
    for (int i=0; i<0x40; i++)
    {
        int yi = (max_y - val[i]) * val_y;
        int out = (i >> 4) & 0x03;
        int bases = i & 0x0F;
        int balls = 0;
        int strikes = 0;
        int xi = out * out_x + bases * base_x + (balls + strikes)*pitch_x;
        for (int j=0; j<0x40; j++)
        {
            if (agg[0x40*i + j] == 0) continue;
            int yj = (max_y - val[j]) * val_y;
            int out = (j >> 4) & 0x03;
            int bases = j & 0x0F;
            int balls = 0;
            int strikes = 0;
            int xj = out * out_x + bases * base_x + (balls + strikes)*pitch_x;
            double dv = (double)RunValue((i|1)<<8) - (double)RunValue(j<<8) + val[j] - val[i];
            int G = (dv > 0.)?255:(4.+dv)*60.;
            int R = (dv < 0.)?255:(4.-dv)*60.;
            int B = 0;
            fprintf (fp, "// from %04X to %04X: dv = %f\n", i<<8, j<<8, dv);
            fprintf (fp, "ctx.strokeStyle=\"#%02X%02X%02X\";\n", R, G, B);
            int w = (int)((double) (20*agg[0x40*i + j]) / (double)sum[i]);
            fprintf (fp, "ctx.lineWidth=%d;\n", MIN(MAX(w,1),7));
            fprintf (fp, "ctx.beginPath();\n");
            fprintf (fp, "ctx.moveTo(%d, %d);\n", xi, yi);
            fprintf (fp, "ctx.lineTo(%d, %d);\n", xj, yj);
            fprintf (fp, "ctx.stroke();\n");
        }
    }
    fprintf (fp, "</script>\n");
    fprintf (fp, "</body>\n");
    fprintf (fp, "</html>\n");
    fclose(fp);
}