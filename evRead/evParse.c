#include "evParse.h"
#include <assert.h> /* for assert() */
#include <stdio.h>
#include <string.h>
#include "EventFields.h"

#define QUOTE ('\"')
#define COMMA (',')
#define NUMFIELDS (97)
//#define NUMFIELDS (16)
/* 16 fields used (0,4,7,10,26-28,34,35,40,58-61,79,96)
	gameid                                 =  0,
	outs                                   =  4,
	pitchsequence                          =  7,
	batter                                 = 10,
	firstrunner                            = 26,
	secondrunner                           = 27,
	thirdrunner                            = 28,
	eventtype                              = 34,
	battereventflag                        = 35,
	outsonplay                             = 40,
	batterdest                             = 58,
	runneron1stdest                        = 59,
	runneron2nddest                        = 60,
	runneron3rddest                        = 61,
	EndGameFlag                            = 79,
	eventnum                               = 96
*/

void Parse (char **f, char* s)
{
	int i=0;
	while (*s)
	{
		if (*s == QUOTE) s++;
		f[i] = s;
		s = strchr(s, COMMA);
		if (s==0)
		{
			s = f[i] + strlen(f[i])-1;
		}
		if (*(s-1) == QUOTE) *(s-1) = 0;
		*s = 0;
		s++;
		i++;
	}
	assert (i<=NUMFIELDS);
}

/* https://www.retrosheet.org/datause.html */
/* FEDCBA9876543210 */
/* OO321HBBBFFFFFSS */
/*  3 outs    : 2 bit 15..14 */
/*  4 runners : 4 bit 13..10 */
/*  7 balls   : 3 bit  9.. 7 */
/* 31 fouls   : 5 bit  6.. 2 */
/*  3 strikes : 2 bit  1.. 0 */
/* total      : 16 bit */
#define DEST(x) (((x)>'0' && (x)<'4')?(1<<(10+((x)&3))):0)
int Process(unsigned short* to, char** f, enum EventType* et)
{
	int OutsStart;
	int OutsOnPlay;
	int BatterEvent;
	int event;
	int Pitch = 0;
	int retval = 0;
	(void) sscanf(f[outs]      , "%d", &OutsStart);
	(void) sscanf(f[outsonplay], "%d", &OutsOnPlay);
	(void) sscanf(f[eventtype] , "%d", &event);
	BatterEvent = (f[battereventflag][0] == 'T')?1:0;

	/* initialize 1st state */
	if (to[retval] == 0xFFFF)
	{
		/* do not use leadoffflag because it is set twice @ non-batter events */
		to[retval]  = (f[thirdrunner] [0]?(1<<13):0);
		to[retval] |= (f[secondrunner][0]?(1<<12):0);
		to[retval] |= (f[firstrunner] [0]?(1<<11):0);
		if (to[retval] != 0)
		{
			/*
			fprintf (stderr, "%s,%s,runners at leadoff\n",
					 f[gameid],f[eventnum]);
			*/
		}
		to[retval+1] = to[retval];
	}
	retval++;
	if ((to[retval-1] & (1<<10)) == 0) /* no batter at home */
	{
		to[retval] = to[retval-1] | (1<<10);
		retval++;
	}
	assert ((to[retval-1] & 0xC000) == (OutsStart << 14));
	assert ((to[retval-1] & 0x2000) == (f[thirdrunner] [0]?(1<<13):0));
	assert ((to[retval-1] & 0x1000) == (f[secondrunner][0]?(1<<12):0));
	assert ((to[retval-1] & 0x0800) == (f[firstrunner] [0]?(1<<11):0));
	assert ((to[retval-1] & 0x0400) == (f[batter]      [0]?(1<<10):0));
	int Strikes = 0;
	int Balls   = 0;
	int Fouls   = 0;
	for (char* p = f[pitchsequence]; *p; p++)
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
				Fouls++;
			break;

			/* Pitches - Strikes */
			case 'A': //  automatic strike, usually for pitch timer violation
			case 'C': //  called strike
			case 'K': //  strike (unknown type)
			case 'M': //  missed bunt attempt
			case 'Q': //  swinging on pitchout
			case 'S': //  swinging strike
				Strikes++;
			break;

			/* Pitches - Balls */
			case 'B': //  ball
			case 'I': //  intentional ball
			case 'P': //  pitchout
			case 'V': //  called ball because pitcher went to his mouth or automatic ball on intentional walk or pitch timer violation
				Balls++;
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
		if (Pitch)
		{
			assert (Balls   <  8);
			assert (Fouls   < 32);
			assert (Strikes <  4);
			to[retval] = to[retval-1] & 0xFC00;
			to[retval] |= (Balls   << 7);
			to[retval] |= (Fouls   << 2);
			to[retval] |= (Strikes << 0);
			if ((to[retval] & 0x3FF) > (to[retval-1] & 0x3FF))
			{
				retval++;
				to[retval] = to[retval-1] & 0xFC00;
			}
			else
			{
				/*
				fprintf (stderr, "%s,%s,skipping pitch\n",
				         f[gameid],f[eventnum]);
				*/
				Pitch = 0;
			}

		}
	}
	if (!BatterEvent)
	{
		if (Pitch) /* take into account event on the pitch */
			retval--;
		to[retval] = 1<<10;
	}
	else
	{
		to[retval] = 0;
	}
	to[retval] |= ((OutsStart+OutsOnPlay)<<14);
	to[retval] |= DEST(f[batterdest][0]);
	to[retval] |= DEST(f[runneron1stdest][0]);
	to[retval] |= DEST(f[runneron2nddest][0]);
	to[retval] |= DEST(f[runneron3rddest][0]);
	to[retval] |= (Balls   << 7);
	to[retval] |= (Fouls   << 2);
	to[retval] |= (Strikes << 0);
	if (BatterEvent)
		to[retval] &= 0xFC00;
	retval++;
	if ((f[EndGameFlag][0] == 'T') ||
	    (OutsStart+OutsOnPlay == 3))
	{
		to[retval] = 0xFFFF;
		retval++;
	}
	*et = (enum EventType) event;
	fprintf (stdout, "%s,%s", f[gameid],f[eventnum]);
	for (int i=0; i<retval; i++)
		fprintf (stdout, ",%04X", to[i]);
	fprintf (stdout, "\n");
	return retval;
}