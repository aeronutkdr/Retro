#include "mystdlib.h"
#include "NodeTree.h"
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
#include "EventFields.h"

/* 2b out   */
/* 4b base  */
/* 8b count */
#define BASE_BITS  (4)
#define BASE_SHIFT (0)
#define OUT_SHIFT (BASE_BITS+BASE_SHIFT)
#define MAXLINE (518)
#define MAX(a,b) (((a)>(b))?(a):(b))

void Parse(FILE* fp);
void DumpEvents(int lc, int* e, int num);
int RunValue(int st);
int Pitches(char* str, int* p, int pc, int* npc);
int ParseLine(char* line, int** evts, int* evtNum, int pc);
int main (int argc, char* argv[])
{
	FILE* fp = fopen(argv[1], "r");
#if 1
	Parse(fp);
	fclose(fp);
	int z = NT_Dump();
	fprintf (stderr, "%d results\n", z);
	NT_DumpBin ("out.bin");
	int count;
	int* freq = NT_CalcFreqs(&count);
	for (int i=0; i<0x3000; i++)
	{
		if (freq[i] != freq[count+i])
			printf ("%04X: %d\t%d\n", i, freq[i], freq[count+i]);
	}
	free(freq);
	NT_Drop();
#else
	int from;
	int to;
	char c;
	to = fscanf (fp, "%X%c", &from, &c);
	while (fscanf(fp, "%X%c", &to, &c) == 2)
	{
		NT_AddRoute(from, to);
		from = to;
	}
	printf ("%d\n", NT_Dump());
	NT_Drop();
#endif
	return 0;
}

int RunValue(int st)
{
	st >>= 8;
	return ((st>>0)&1) +
	       ((st>>1)&1) +
	       ((st>>2)&1) +
	       ((st>>3)&1) +
	       ((st>>4)&3);
}

void Parse(FILE* fp)
{
	char line[MAXLINE + 1];
	int CountStart = -1;
	int StartAB;
	int lineCount = 0;
	int stderrcount = 0;
	while (fgets(line, MAXLINE, fp))
	{
		lineCount++;
		if (lineCount==4827)
		{
			//printf ("break\n");
		}
		int* events;
		int numEvents;
		StartAB = CountStart == -1;
		assert (CountStart);
		if (CountStart == -1)
		{
			CountStart = 0;
		}
		else
		{
			CountStart--;
		}
		CountStart = ParseLine(line, &events, &numEvents, CountStart);
		if ((CountStart != -1) && !StartAB)
		{
			CountStart++;
		}
		stderrcount++;
		if (stderrcount == 9309)
		{
			printf ("stop\n");
		}
		fprintf (stderr, "%04X\n", events[0]);
		if (StartAB)
		{
			assert (events[0] & 0x100);
			NT_AddRoute((events[0]& ~0x100),events[0]);
			//fprintf (stderr, "%04X **\n", (events[0]& ~0x100));
			//if (CountStart) CountStart--;
		}
		for (int i=0; i<numEvents-1; i++)
		{
			if (stderrcount == 9308)
			{
				printf ("stop\n");
			}
			NT_AddRoute(events[i], events[i+1]);
			fprintf (stderr, "%04X\n", events[i+1]);
			stderrcount++;
		}
		free (events);
	}
#if 0
	struct Node *p = start;
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
		freq[From*NUM_STATES+From] -= Sum;
		count++;
		free(pitch);
	}
#endif
}

int ParseLine(char* line, int** evts, int* evtNum, int pc)
{
	char*            tok      = strtok(line, ",");
	unsigned int     From     = 0;
	unsigned int     To       = 0;
	int              pitch_sz = 0;
	int              *pitches = 0;
	int              bev      = 0;
	enum EventFields evt      = gameid;
	int              npc      = 0;
	char*            pitchseq = 0;
	while (tok)
	{
		switch (evt)
		{
			case outs:            From += ((*tok - '0') << OUT_SHIFT);
			                      To   += ((*tok - '0') << OUT_SHIFT);        break;
			case batter:          From += (tok[1] != '\"')?(1<<BASE_SHIFT):0; break;
			case firstrunner:     From += (tok[1] != '\"')?(2<<BASE_SHIFT):0; break;
			case secondrunner:    From += (tok[1] != '\"')?(4<<BASE_SHIFT):0; break;
			case thirdrunner:     From += (tok[1] != '\"')?(8<<BASE_SHIFT):0; break;
			case outsonplay:      To   += ((*tok - '0') << OUT_SHIFT);        break;
			case battereventflag: bev   = (tok[1] == 'T');
			                      To   += (1-bev)<<BASE_SHIFT;                break;
			case leadoffflag:     if (tok[1]=='T') pc = 0;                    break;
			case pitchsequence:   pitchseq = tok;                             break;
			case batterdest:
			case runneron1stdest:
			case runneron2nddest:
			case runneron3rddest:
				switch (*tok)
				{
					case '1': To += (2<<BASE_SHIFT); break;
					case '2': To += (4<<BASE_SHIFT); break;
					case '3': To += (8<<BASE_SHIFT); break;
				}
			break;
			default:                                                          break;
		}
		tok = strtok(NULL, ",");
		evt++;
	}
	if (pitchseq)
	{
		pitch_sz = (strlen(pitchseq));
		pitches = malloc(sizeof(int)*pitch_sz);
		pc = Pitches(pitchseq+1, pitches, pc, &npc);
		assert(pc<=pitch_sz);
	}
	assert(pc+1>bev);
	assert(evt==eventnum+1);
	*evtNum = MAX(1, pc);
	*evts = malloc(sizeof(int) * ((*evtNum) + 1));
	for (int i=0; i<((*evtNum)-1); i++)
	{
		(*evts)[i] = (From << 8) | pitches[i];
	}
	if (bev || (pc==0)) (*evts)[(*evtNum)-1] = (To << 8);
	else                (*evts)[(*evtNum)-1] = (To << 8) | pitches[pc-1];
	free(pitches);
	return bev?-1:(pc+npc);
}

int Pitches(char* str, int* p, int pc, int* npc)
{
	int retval = 0;
	int count = 0;
	*npc = 0;
	if (pc==0) p[retval++] = 0;
	while (*str != '\"')
	{
		if (pc>0) pc--;
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
			(*npc)++;
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
				p[retval] = count;
				if (pc==0) retval++;
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
				p[retval] = count;
				if (pc==0) retval++;
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

void DumpEvents(int lc, int* e, int num)
{
	for (int i=0; i<num; i++)
	{
		printf ("%d: %1d %01X %1d-%d\n", lc,
		                                 ((e[i]>>12)&0x03),
		                                 ((e[i]>> 8)&0x0F),
                                             ((e[i]>> 5)&0x03),
                                             ((e[i]    )&0x1F));
	}
	//printf ("*****\n");
}