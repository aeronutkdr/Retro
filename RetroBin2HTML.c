#include "mystdlib.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>

struct ToType
{
	unsigned short State;
	unsigned short Pad;
	unsigned int   Freq;
};
struct StateType
{
	unsigned short State;
	unsigned short NumTos;
	unsigned int   Value;
	unsigned int   FreqIn;
};

#define ToColor(a) ((int)(((a)>0.)?255.:(((a)>-3.)?(255.+(a)*85.):0.)))
int RunValue(unsigned short st)
{
	return ((st>>(0+8))&1) +
	       ((st>>(1+8))&1) +
	       ((st>>(2+8))&1) +
	       ((st>>(3+8))&1) +
	       ((st>>(4+8))&3);
}

int main(int argc, char* argv[])
{
    const int pitch_x = 15;
    const int base_x = pitch_x * 10;
    const int out_x = base_x * 16;
    const int val_y = 750;
    const double max_y = 4.0;
    const double max_x = 3.95;
    const int dx = 15;
    FILE* fp = fopen (argv[1], "rb");
    fseek(fp, 0, SEEK_END);
    long len = ftell(fp);
    unsigned char* fbin = malloc(len);
    fseek(fp, 0, SEEK_SET);
    fread(fbin, 1, len, fp);
    fclose(fp);
    struct StateType* stateptrs[0x4000] = {0};
    int slen = strlen(argv[1]);
    char* outf = malloc(slen+2);
    memcpy(outf, argv[1], slen);
    memcpy(outf+slen-3,"html\x00", 5);
    fp = fopen(outf, "w");

    fprintf (fp, "<!DOCTYPE html>\n");
    fprintf (fp, "<html>\n");
    fprintf (fp, "<body>\n");
    fprintf (fp, "<h1>%s</h1>\n", outf);
    free(outf);
    fprintf (fp, "<canvas id=\"myCanvas\" width=\"%d\" height=\"%d\" style=\"border:1px solid grey;\"></canvas>\n",
             (int) (out_x*max_x),(int) (max_y*val_y));
    fprintf (fp, "<script>\n");
    fprintf (fp, "const canvas = document.getElementById(\"myCanvas\");\n");
    fprintf (fp, "const ctx = canvas.getContext(\"2d\");\n");
    fprintf (fp, "ctx.font = \"25px serif\";\n");
    unsigned char* p=fbin;
    while (p < fbin+len)
    {
        assert (((int)p&3)==0);
        struct StateType* s = (struct StateType*) p;
        stateptrs[s->State] = s;
        p += s->NumTos*sizeof(struct ToType) + sizeof(struct StateType);
    }

    p = fbin;
    while (p < fbin+len)
    {
        assert (((int)p&3)==0);
        struct StateType* s = (struct StateType*) p;
        //if (((s->State & 0x1F) < 0x03) && ((s->State&0xE0)<0x80))
        {
            int out = (s->State >> 12) & 0x03;
            int bases = (s->State >> 8) & 0x0F;
            int balls = (s->State >> 5) & 0x07;
            int strikes = (s->State) & 0x1F;
            int xi = out * out_x + bases * base_x + (balls + strikes)*pitch_x + (dx>>1);
            double d = (double)s->Value/(double)(1<<29);
            int yi = (int)((max_y - d) * (double)val_y);
            for (int i=0; i<s->NumTos; i++)
            {
                struct ToType* t = (struct ToType*) (p+i*sizeof(struct ToType) + sizeof(struct StateType));
                out = (t->State >> 12) & 0x03;
                bases = (t->State >> 8) & 0x0F;
                balls = (t->State >> 5) & 0x07;
                strikes = (t->State) & 0x1F;
                int xj = out * out_x + bases * base_x + (balls + strikes)*pitch_x + (dx>>1);
                d = (double)stateptrs[t->State]->Value/(double)(1<<29);
                int yj = (int)((max_y - d) * (double)val_y);
                double dv = (double)RunValue(s->State|0x100) - (double)RunValue(t->State);
                dv += ((double)stateptrs[t->State]->Value - (double)stateptrs[s->State]->Value)/(double)(1<<29);
                int G = ToColor(dv);
                int R = ToColor(-dv);
                int B = 0;
                fprintf (fp, "// from %04X to %04X: dv = %f\n", s->State, t->State, dv);
                fprintf (fp, "ctx.strokeStyle=\"#%02X%02X%02X\";\n", R, G, B);
                fprintf (fp, "// frequency = %d\n", t->Freq);
                int w = (int)(log((double)t->Freq)/log(2.))+1;
                fprintf (fp, "ctx.lineWidth=%d;\n", w);
                fprintf (fp, "ctx.beginPath();\n");
                fprintf (fp, "ctx.moveTo(%d, %d);\n", xi, yi);
                if (RunValue(s->State|0x100) > RunValue(t->State))
                {
                    fprintf (fp, "ctx.lineTo(%d, %d);\n", xj, yi);
                }
                fprintf (fp, "ctx.lineTo(%d, %d);\n", xj, yj);
                fprintf (fp, "ctx.stroke();\n");
            }
            fprintf (fp, "ctx.fillText(\"%04X\", %d, %d);\n", s->State, xi, yi);
        }
        p += s->NumTos*sizeof(struct ToType) + sizeof(struct StateType);
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
    fprintf (fp, "</script>\n");
    fprintf (fp, "</body>\n");
    fprintf (fp, "</html>\n");
    fclose(fp);
    free(fbin);
    printf ("%d - %d = %d\n", mallocctr, freectr, mallocctr-freectr);
    return 0;
}