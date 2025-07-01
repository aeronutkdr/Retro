#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define LINE_LEN (200)
#define PITCH_LEN (200)
#define PLAY_LEN (200)
struct LineType
{
    char inning[3];
    char out;
    char batter[9];
    char count[3];
    char pitches[PITCH_LEN];
    char play[PLAY_LEN];
};
struct GameType
{
    int inning;
    int bottom;
    char batter[9];
    int balls;
    int strikes;
    char pitches[PITCH_LEN];
    char play[PLAY_LEN];
    char outs;
};
void ProcessPA(struct LineType *pL);
void ProcessPlay(struct GameType* g, char* str, int Num);
int main (int argc, char* argv[])
{
    FILE* fp = fopen (argv[1], "r");
    char line[LINE_LEN+1];
    struct LineType data;
    struct LineType dataNext = {0};
    struct GameType game = {0};
    int lineNum = 0;

    while (fgets(line, LINE_LEN, fp) != NULL)
    {
        lineNum++;
#if 1 
        if (strncmp(line,"play,", 5)==0)
            ProcessPlay(&game, line+5, lineNum);
#else
        memcpy(&data, &dataNext, sizeof (struct LineType));
        if (strncmp(line,"play,", 5)==0)
        {
            char* p = strchr(line,',')+1;
            char* pnext = strchr(p,',');
            strncpy(dataNext.inning, p, pnext-p);
            dataNext.inning[pnext-p] = 0;
            p = pnext+1;
            pnext = strchr(p,',');
            dataNext.out = (*p)-'0';
            p = pnext+1;
            pnext = strchr(p,',');
            strncpy(dataNext.batter, p, pnext-p);
            dataNext.batter[pnext-p] = 0;
            p = pnext+1;
            pnext = strchr(p,',');
            strncpy(dataNext.count, p, pnext-p);
            dataNext.count[pnext-p] = 0;
            p = pnext+1;
            pnext = strchr(p,',');
            strncpy(dataNext.pitches, p, pnext-p);
            dataNext.pitches[pnext-p] = 0;
            p = pnext+1;
            pnext = strchr(p,'\n');
            strncpy(dataNext.play, p, pnext-p);
            dataNext.play[pnext-p] = 0;
            if (strcmp(data.batter, dataNext.batter) == 0)
            {
                strcat(data.play,".");
                strcat(data.play,dataNext.play);
                strcpy(dataNext.play, data.play);
            }
            else
            {
                // process this complete Plate Appearance
                ProcessPA(&data);
            }
        }
#endif
    }
    return 0;
}
void ProcessPA(struct LineType *pL)
{
    fprintf (stdout, "%s,", pL->inning);
    fprintf (stdout, "%d,", pL->out);
    fprintf (stdout, "%s,", pL->inning);
    fprintf (stdout, "%s,", pL->count);
    fprintf (stdout, "%s,", pL->pitches);
    fprintf (stdout, "%s\n", pL->play);
}

#define MIN(a,b) (((a)<(b))?(a):(b))
#define printif(a) if((a)) printf("line = %d\n", Num)
void ProcessPlay(struct GameType* g, char* str, int Num)
{
    struct GameType lastg;
    memcpy (&lastg, g, sizeof (struct GameType));
    char *p;
    int i;
    char b, s;
    g->batter[0] = 0;
    g->pitches[0] = 0;
    g->play[0] = 0;
#if 0
    if (Num == 142)
    {
        printf ("here\n");
    }
#endif
    for (i=0, p=str; i<5; i++, p++)
    {
        char* pnext = strchr(p,',');
        pnext[0] = 0;
        switch (i)
        {
            case 0: sscanf(p, "%d", &g->inning); break;
            case 1: sscanf(p, "%d", &g->bottom); break;
            case 2: sscanf(p, "%s", g->batter ); break;
            case 3: sscanf(p, "%c%c", &b, &s  ); break;
            case 4: sscanf(p, "%s", g->pitches); break;
        }
        p = pnext;
    }
    strchr(p,'\n')[0] = 0;
    sscanf(p, "%s", g->play);
    g->balls = (int) (b-'0');
    g->strikes = (int) (s-'0');
    char* lastdot = strrchr(g->pitches, '.');
    b = 0;
    s = 0;
    char f = 0;
    int Pitch;
    for (p = g->pitches; *p; p++)
    {
		Pitch = 1;
        switch (*p)
        {
			/* Pitches - Fouls */
			case 'F': //  foul
			case 'L': //  foul bunt
			case 'O': //  foul tip on bunt
			case 'R': //  foul ball on pitchout
			case 'T': //  foul tip
				f += (p[1] != 0) || !strcmp(g->play, "NP");
			break;

			/* Pitches - Strikes */
			case 'A': //  automatic strike, usually for pitch timer violation
			case 'C': //  called strike
			case 'K': //  strike (unknown type)
			case 'M': //  missed bunt attempt
			case 'Q': //  swinging on pitchout
			case 'S': //  swinging strike
				s += (p[1] != 0) || !strcmp(g->play, "NP");
			break;

			/* Pitches - Balls */
			case 'B': //  ball
			case 'I': //  intentional ball
			case 'P': //  pitchout
			case 'V': //  called ball because pitcher went to his mouth or automatic ball on intentional walk or pitch timer violation
				b += (p[1] != 0) || !strcmp(g->play, "NP");
			break;

			/* Non Pitches */
			case 'H': //  hit batter
			case 'N': //  no pitch (on balks and interference calls)
			case 'U': //  unknown or missed pitch
			case '+': //  following pickoff throw by the catcher
			case '*': //  indicates the following pitch was blocked by the catcher
			case '.': //  marker for play not involving the batter
			case '1': //  pickoff throw to first
			case '2': //  pickoff throw to second
			case '3': //  pickoff throw to third
			case '>': //  Indicates a runner going on the pitch
			/* In Play */
			case 'X': //  ball put into play by batter
			case 'Y': //  ball put into play on pitchout
				Pitch = 0;
			break;

			default :
				assert(0);
			break;
        }
        if (p>lastdot)
        {
            //fprintf (stdout, "use this\n");
        }
#if 0
        else if (p==lastdot)
        {
            if ((lastg.balls   != b) ||
                (lastg.strikes != s))
            {
                /* TODO: no match if . is for (new) sub */
                //assert (lastg.balls == b);
                //assert (lastg.strikes == s);
            }
        }
#endif
        else
        {
            //fprintf (stdout, "do not use this\n");
        }
    }
    printif(g->balls   != MIN(3,b));
    printif(g->strikes != MIN(2,s+f));
#if 0
    if ((g->balls   != MIN(3,b)) ||
        (g->strikes != MIN(2,s+f)))
    {
        /* TODO won't match for caught stealing */
        assert ((g->balls == MIN(3,b)));
        assert (g->strikes == MIN(2,s+f));
    }
#endif
}