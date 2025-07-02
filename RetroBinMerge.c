#include "NodeTree.h"
#include "GJ.h"
#include "mystdlib.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    FILE* fp = fopen (argv[1], "r");
    int num;
    char line[40];
    while (fgets(line, 40, fp))
    {
        char* c = strchr(line,'\n');
        if (c) *c = 0;
        printf ("%s\n", line);
        NT_LoadBin(line);
    }
    fclose(fp);
    NT_Dump();
    struct NT_SolveType* sol = NT_Solve(&num);
    if (sol)
    {
        for (int i=0; i<num; i++)
        {
            printf ("%04X: %1.4f\n", sol[i].state, sol[i].value);
        }
        free(sol);
    }
    NT_Drop();
    printf ("%d - %d = %d\n", mallocctr, freectr, mallocctr-freectr);
    return 0;
}

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
    const double max_y = 3.5;
    const double max_x = 3.95;
    int dx = 15;
    fprintf (fp, "<html>\n");
    fprintf (fp, "<body>\n");
    fprintf (fp, "<h1>%s</h1>\n", fname);
    fprintf (fp, "<canvas id=\"myCanvas\" width=\"%d\" height=\"%d\" style=\"border:1px solid grey;\"></canvas>\n",
		 (int) (out_x*max_x),(int) (max_y*val_y));
    fprintf (fp, "<script>\n");
    fprintf (fp, "const canvas = document.getElementById(\"myCanvas\");\n");
    fprintf (fp, "const ctx = canvas.getContext(\"2d\");\n");
    fprintf (fp, "ctx.font = \"25px serif\";\n");
    //for (int i=0; )
    for (int i=0; i<num; i++)
    {
        if ((sol[i].state & 0xFF) != 0) continue;
        val[sol[i].state>>8] = sol[i].value;
    }
    for (int i=0; i<0x40; i++)
    {
        int yi = (max_y - val[i]) * val_y;
        int out = (i >> 4) & 0x03;
        int bases = i & 0x0F;
        int balls = 0;
        int strikes = 0;
        int xi = out * out_x + bases * base_x + (balls + strikes)*pitch_x + dx;
        for (int j=0; j<0x40; j++)
        {
            if (agg[0x40*i + j] == 0) continue;
            int yj = (max_y - val[j]) * val_y;
            int out = (j >> 4) & 0x03;
            int bases = j & 0x0F;
            int balls = 0;
            int strikes = 0;
            int xj = out * out_x + bases * base_x + (balls + strikes)*pitch_x + dx;
            double dv = (double)RunValue((i|1)<<8) - (double)RunValue(j<<8) + val[j] - val[i];
            int G = ToColor(dv);
            int R = ToColor(-dv);
            int B = 0;
            fprintf (fp, "// from %02X to %02X: dv = %f\n", i, j, dv);
            fprintf (fp, "ctx.strokeStyle=\"#%02X%02X%02X\";\n", R, G, B);
            fprintf (fp, "// frequency = %d\n", agg[0x40*i + j]);
            int w = (int)(log((double)agg[0x40*i+j])/log(2.))+1;
            fprintf (fp, "ctx.lineWidth=%d;\n", w);
            fprintf (fp, "ctx.beginPath();\n");
            fprintf (fp, "ctx.moveTo(%d, %d);\n", xi, yi);
            if (RunValue((i|1)<<8) > RunValue(j<<8))
            {
                fprintf (fp, "ctx.lineTo(%d, %d);\n", xj, yi);
            }
            fprintf (fp, "ctx.lineTo(%d, %d);\n", xj, yj);
            fprintf (fp, "ctx.stroke();\n");
        }
    }
    fprintf (fp, "// gridlines...\n");
    fprintf (fp, "ctx.strokeStyle=\"#%02X%02X%02X\";\n", 0, 0, 0);
    fprintf (fp, "ctx.lineWidth=%d;\n", 1);
    for (double i=max_y*val_y; i>0; i-=val_y/2)
    {
        fprintf (fp, "ctx.beginPath();\n");
        fprintf (fp, "ctx.moveTo(%d, %d);\n", 0, (int)i);
        fprintf (fp, "ctx.lineTo(%d, %d);\n", (int)(out_x*max_x), (int)i);
        fprintf (fp, "ctx.stroke();\n");
    }
    fprintf (fp, "// state labels...\n");
    for (int i=0; i<num; i++)
    {
        if ((sol[i].state & 0xFF) != 0) continue;
        int yi = (max_y - sol[i].value) * val_y;
        int out = (sol[i].state >> 12) & 0x03;
        int bases = (sol[i].state >> 8) & 0x0F;
        int balls = (sol[i].state >> 5) & 0x07;
        int strikes = (sol[i].state) & 0x1F;
        int xi = out * out_x + bases * base_x + (balls + strikes)*pitch_x + (dx>>1);
        fprintf (fp, "ctx.fillText(\"%02X\", %d, %d);\n", sol[i].state>>8, xi, yi);
    }
    fprintf (fp, "</script>\n");
    fprintf (fp, "</body>\n");
    fprintf (fp, "</html>\n");
    fclose(fp);
}
