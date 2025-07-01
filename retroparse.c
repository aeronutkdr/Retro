#include "EventFields.h"
#include "Retroparse.h"
#include "NodeTree.h"
#include <assert.h>
#include <string.h>

/* 2b out   */
/* 4b base  */
/* 8b count */
#define BASE_BITS  (4)
#define BASE_SHIFT (0)
#define OUT_SHIFT (BASE_BITS+BASE_SHIFT)
#define MAXLINE (518)
static void RP_ParsePitches(char* str, char From);
static void RP_ParseLine(char* line);

#define MAX_EVENTS (100)
struct LineData
{
	unsigned short evts[MAX_EVENTS];
	char sz;
	char complete;
};
static struct LineData LineEvents = {.sz = 0, .complete = 1};
static int lineCount = 0;
void RP_Parse(FILE* fp)
{
	char line[MAXLINE + 1];
	lineCount = 0;
	while (fgets(line, MAXLINE, fp))
	{
		assert (line[strlen(line)-1] == '\n');
		lineCount++;
		if (lineCount == 4510)
		{
			lineCount *= 1;
		}
		RP_ParseLine(line);
		if (LineEvents.complete)
		{
			if (LineEvents.evts[0] & 0x100);
			else
			{
				assert(0);
			}
			NT_AddRoute((LineEvents.evts[0]&(~0x100)),LineEvents.evts[0], 1);
			for (int i=0; i<LineEvents.sz-1; i++)
			{
				NT_AddRoute(LineEvents.evts[i],LineEvents.evts[i+1], 1);
			}
		}
	}
}
static void RP_ParseLine(char* line)
{
	char*            tok      = strtok(line, ",");
	unsigned short   From     = 0;
	unsigned short   To       = 0;
	int              bev      = 0;
	enum EventFields evt      = gameid;
	char*            pitchseq = 0;
	static int LastPitch = 0;
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
			case battereventflag:
			case EndGameFlag:     bev  |= (tok[1] == 'T');
			                      To |= (1-bev)<<BASE_SHIFT;                  break;
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
	assert(evt==eventnum+1);
	if (LineEvents.complete)
	{
		LineEvents.evts[0] = From<<8;
		LineEvents.sz = 1;
		LineEvents.complete = 0;
		LastPitch = 0;
	}
	if (strlen(pitchseq) == 2) // empty
	{
		fprintf (stderr, "empty pitch sequence: line %d\n", lineCount);
		LineEvents.evts[(int)LineEvents.sz] = (To<<8);
		if (LineEvents.sz)
			LineEvents.evts[(int)LineEvents.sz] |= (LineEvents.evts[LineEvents.sz-1] & 0xFF);
		LineEvents.sz++;
		LineEvents.complete = (bev || ((To & 0x30) == 0x30));
	}
	else
	{
		RP_ParsePitches(pitchseq+LastPitch, From);
		LineEvents.complete = (bev || ((To & 0x30) == 0x30));
		if (LineEvents.complete)
		{
			if (LineEvents.sz == 1) LineEvents.sz++;
			LineEvents.evts[LineEvents.sz-1] = (To<<8);
		}
		else
		{
			LineEvents.evts[LineEvents.sz-1] &= 0xFF;
			LineEvents.evts[LineEvents.sz-1] |= (To<<8);
			LastPitch = strlen(pitchseq)-2;
		}
	}
}

void RP_ParsePitches(char* str, char From)
{
	unsigned char count = LineEvents.evts[LineEvents.sz-1] & 0xFF;
	char Pitch;
	str++;
	for (;(*str != '\"');str++)
	{
		Pitch = 1;
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
			Pitch = 0;
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
			/* limited to 31 strikes */
			if ((count&0x1F) < 0x1F) count++;
		break;
		case 'X': /* ball put into play by batter */
		case 'Y': /* ball put into play on pitchout */
			count = 0;
		break;

		case 'P': /* pitchout */
		case 'B': /* ball */
		case 'H': /* hit batter */
		case 'I': /* intentional ball */
		case 'V': /* called ball because pitcher went to his mouth or automatic ball on intentional walk or */
		case 'p': /* pitch timer violation */
			/* limited to 7 balls */
			if ((count&0xE0) < 0xE0) count+=0x20;
		break;

		default:
			assert(0);
		break;
		}
		if (Pitch)
		{
			LineEvents.evts[(int)LineEvents.sz] = (From<<8) | count;
			LineEvents.sz++;
		}
	}
}