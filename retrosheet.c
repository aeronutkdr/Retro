/*
each State is defined by Outs + 3B + 2B + 1B + HP + Count
for all i states
{
	StateVal[i] = StateUsed(i)
}
for each StateTransition
{
	Freq[From][To]++
	if ABEnd
		Event[From][To]++
}
for all From States
	for all To States
		Run[From] += StateVal[From]*Freq[From][To];
		Run[To]   -= StateVal[To]*Freq[From][To];
		Event[From][To|1] = Freq[From][To];
		EventRun += StateVal[From]*Event[From][To];
		EventRun -= StateVal[To]*Event[From][To];
*/
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "mystdlib.h"
#include "EventFields.h"

struct PrevStateType
{
	unsigned int State;
	unsigned int PitchCount;
};

/* 2b out   */
/* 4b base  */
/* 8b count */
#define OUT_BITS   (2)
#define BASE_BITS  (4)
#define COUNT_BITS (8)
#define OUT_NUM   (1<<(OUT_BITS))
#define BASE_NUM  (1<<(BASE_BITS))
#define COUNT_NUM (1<<(COUNT_BITS))
#define OUT_MASK   ((OUT_NUM-1) << (BASE_BITS+COUNT_BITS))
#define BASE_MASK  ((BASE_NUM-1) << (COUNT_BITS))
#define COUNT_MASK ((COUNT_NUM-1) << (0))
#define NUM_STATES (OUT_NUM * BASE_NUM * COUNT_NUM)
void Parse(FILE* fp, int* freq);
void Dump(int* freq);
void DumpStr(char* str);
void DumpPitch(int* p, int num, int sz, int ln);
int RunValue(int st);
int Pitches(char* str, int* p);
int frequency[NUM_STATES*NUM_STATES];
int main (int argc, char* argv[])
{
	memset(frequency,0,sizeof(int)*NUM_STATES*NUM_STATES);
	FILE* fp = fopen(argv[1], "r");
	Parse(fp, frequency);
	fclose(fp);
	Dump(frequency);
	return 0;
}

int RunValue(int st)
{
	return ((st>>(0+8))&1) +
	       ((st>>(1+8))&1) +
	       ((st>>(2+8))&1) +
	       ((st>>(3+8))&1) +
	       ((st>>(4+8))&3);
}

#define DumpStr(s)
#if 0
void DumpStr(char* str)
{
	printf ("%s\n", str);
}
#endif

void Dump(int* freq)
{
	for (int i=0; i<NUM_STATES; i++)
	{
		int Sum = 0; //freq[NUM_STATES*i];
		int Run = 0; //(RunValue(i))*freq[NUM_STATES*i];
		//printf ("%d", freq[NUM_STATES*i]);
		for (int j=0; j<NUM_STATES; j++)
		{
			Run += (RunValue(i)-RunValue(j))*freq[NUM_STATES*i+j];
			Sum += freq[NUM_STATES*i+j];
			//printf ("\t%d", freq[NUM_STATES*i+j]);
		}
		printf ("\t%d\n", Run);
		assert(Sum==0);
	}
}

#define MAXLINE (518)
/* have to deal with:
   if previous Parse did not end in batter event,
   then there is a starting count and some pitches
   have already been consumed */
void Parse(FILE* fp, int* freq)
{
	//static struct PrevStateType prevState = {.PitchCount = 0, .State = 0};
	int count = 0;
	char line[MAXLINE + 1];
	while (fgets(line, MAXLINE, fp))
	{
		char* tok = strtok(line, ",");
		enum EventFields evt = gameid;
		unsigned int From = 0;
		unsigned int To = 0;
		unsigned char numPitches = 0;
		int* pitch = 0;
		int pitch_sz = 0;
		while (tok)
		{
			switch (evt)
			{
				case outs:
					DumpStr(tok);
					From += (*tok - '0') * (16*256);
					To   += (*tok - '0') * (16*256);
				break;
				case batter:
					DumpStr(tok);
					From += (tok[1] != '\"')?(1*256):0;
				break;
				case firstrunner:
					DumpStr(tok);
					From += (tok[1] != '\"')?(2*256):0;
				break;
				case secondrunner:
					DumpStr(tok);
					From += (tok[1] != '\"')?(4*256):0;
				break;
				case thirdrunner:
					DumpStr(tok);
					From += (tok[1] != '\"')?(8*256):0;
				break;
				case outsonplay:
					DumpStr(tok);
					To += (*tok - '0') * (16*256);
				break;
				case batterdest:
				case runneron1stdest:
				case runneron2nddest:
				case runneron3rddest:
					DumpStr(tok);
					switch (*tok)
					{
						case '1': To += (2*256); break;
						case '2': To += (4*256); break;
						case '3': To += (8*256); break;
					}
				break;
				case pitchsequence:
					pitch_sz = (strlen(tok)-2);
					pitch = malloc(sizeof(int)*pitch_sz);
					numPitches = Pitches(tok+1, pitch);
					assert(numPitches<=pitch_sz);
				break;
				case battereventflag:
					if (tok[1] == 'T')
					{
						assert(numPitches>0);
						numPitches--;
					}
				break;
				default:
				break;
			}
			tok = strtok(NULL, ",");
			evt++;
		}
		DumpPitch(pitch, numPitches, pitch_sz, count);
		assert(evt==eventnum+1);
		int f = From*NUM_STATES;
		for (int i=0; i<numPitches; i++)
		{
			int to = From + pitch[i];
			freq[f+to]++;
			f = to*NUM_STATES;
		}
		freq[f+To]++;
		int Sum = 0;
		for (int i=0; i<NUM_STATES; i++)
		{
			Sum += freq[NUM_STATES*From+i];
		}
		freq[From*NUM_STATES+From] -= Sum;
		count++;
		free(pitch);
	}
}

int Pitches(char* str, int* p)
{
	int retval = 0;
	int count = 0;
	while (*str != '\"')
	{
		switch (*str)
		{
		case '+': /* following pickoff throw by the catcher */
		case '*': /* indicates the following pitch was blocked by the catcher */
		case '.': /* marker for play not involving the batter */
		case '1': /* pickoff throw to first */
		case '2': /* pickoff throw to second */
		case '3': /* pickoff throw to third */
		case '>': /* Indicates a runner going on the pitch */
		case 'U': /* unknown or missed pitch */
		case 'N': /* no pitch (on balks and interference calls) */
		break;

		case 'A': /* automatic strike, usually for pitch timer violation */
		case 'C': /* called strike */
		case 'F': /* foul */
		case 'K': /* strike (unknown type) */
		case 'L': /* foul bunt */
		case 'M': /* missed bunt attempt */
		case 'O': /* foul tip on bunt */
		case 'Q': /* swinging on pitchout */
		case 'R': /* foul ball on pitchout */
		case 'S': /* swinging strike */
		case 'T': /* foul tip */
		case 'X': /* ball put into play by batter */
		case 'Y': /* ball put into play on pitchout */
			/* limited to 31 strikes */
			if ((count&0x1F) < 0x1F)
			{
				count++;
				p[retval++] = count;
			}
		break;

		case 'P': /* pitchout */
		case 'B': /* ball */
		case 'H': /* hit batter */
		case 'I': /* intentional ball */
		case 'V': /* called ball because pitcher went to his mouth or automatic ball on intentional walk or */
		case 'p': /* pitch timer violation */
			/* limited to 7 balls */
			if ((count&0xE0) < 0xE0)
			{
				count+=0x20;
				p[retval++] = count;
			}
		break;

		default:
			assert(0);
		break;
		}
		str++;
	}
	return retval;
}

void DumpPitch(int* p, int num, int sz, int ln)
{
	printf ("%5d -> %1d ", ln, sz-num);
	for (int i=0; i<num; i++)
	{
		//printf ("%02X ", p[i]);
		printf ("%1d-%1d ", (p[i]>>5), (p[i]&0x1F));
	}
	printf("\n");
}