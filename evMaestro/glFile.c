#include "glFile.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LINE_LEN (2047)
struct GameLogType
{
	struct GLFileType  log;
	struct GameLogType *next;
};
struct GameLogType* GLstart = NULL;
struct GameLogType* GLend = NULL;
void GLFile_ProcessCompletion(struct GLCompletion* p, char* s)
{
	if (*s == 0)
	{
		memset (p, 0, sizeof (struct GLCompletion));
		return;
	}
	int i = 0;
	char* here = s;
	char* next;
	int ival;
	for (; i<5; here = next+1, i++)
	{
		if (i<4)
		{
			next = strchr(here, ',');
			*next = 0;
		}
		switch (i)
		{
			case   0: strcpy(p->Date, here); break;
			case   1: strcpy(p->Park, here); break;
			case   2: sscanf (here, "%d", &ival); p->vs  = ival; break;
			case   3: sscanf (here, "%d", &ival); p->hs  = ival; break;
			case   4: sscanf (here, "%d", &ival); p->len = ival; break;
		}
	}
}
void GLFile_Open (char* name)
{
	FILE* fp = fopen (name, "r");
	char Line[LINE_LEN + 1];
	while (fgets(Line, LINE_LEN, fp) != NULL)
	{
		if (GLstart == NULL)
		{
			GLstart = malloc (sizeof(struct GameLogType));
			GLend = GLstart;
		}
		else
		{
			GLend->next = malloc (sizeof(struct GameLogType));
			GLend = GLend->next;
		}
		GLend->next = NULL;
		int i = 1;
		char* here = Line;
		char* next;
		int ival = 0;
		for (; i<162; here = next+1, i++)
		{
			if (here[0] == '\"')
			{
				here++;
				next = strchr(here,'\"');
				*next = 0;
				next++;
			}
			else
			{
			      next = strchr(here, ',');
				*next = 0;
			}
			switch (i)
			{
				case   1: strcpy(GLend->log.Date, here); break;
				case   2: GLend->log.NumGames = *here; break;
				case   3: strcpy(GLend->log.DayOfWeek, here); break;
				case   4: strcpy(GLend->log.Visit.Team, here); break;
				case   5: strcpy(GLend->log.Visit.League, here); break;
				case   6: sscanf (here, "%d", &ival); GLend->log.Visit.TeamGameNum = ival; break;
				case   7: strcpy(GLend->log.Home.Team, here); break;
				case   8: strcpy(GLend->log.Home.League, here); break;
				case   9: sscanf (here, "%d", &ival); GLend->log.Home.TeamGameNum = ival; break;
				case  10: sscanf (here, "%d", &ival); GLend->log.Visit.TeamScore = ival; break;
				case  11: sscanf (here, "%d", &ival); GLend->log.Home.TeamScore = ival; break;
				case  12: sscanf (here, "%d", &ival); GLend->log.NumOuts = ival; break;
				case  13: GLend->log.DayNight = *here; break;
				case  14: GLFile_ProcessCompletion(&GLend->log.Completion, here); break;
				case  15: GLend->log.ForfeitInfo = *here; break;
				case  16: break; /* TODO */
				case  17: strcpy(GLend->log.ParkID, here); break;
				case  18: sscanf (here, "%d", &ival); GLend->log.Attendance = ival; break;
				case  19: sscanf (here, "%d", &ival); GLend->log.TimeOfGame = ival; break;
				case  20: strcpy(GLend->log.Visit.LineScore, here); break;
				case  21: strcpy(GLend->log.Home.LineScore, here); break;
				case  22: sscanf (here, "%d", &ival); GLend->log.Visit.OffenseStats.AB = ival; break;
				case  23: sscanf (here, "%d", &ival); GLend->log.Visit.OffenseStats.H = ival; break;
				case  24: sscanf (here, "%d", &ival); GLend->log.Visit.OffenseStats.B2 = ival; break;
				case  25: sscanf (here, "%d", &ival); GLend->log.Visit.OffenseStats.B3 = ival; break;
				case  26: sscanf (here, "%d", &ival); GLend->log.Visit.OffenseStats.HR = ival; break;
				case  27: sscanf (here, "%d", &ival); GLend->log.Visit.OffenseStats.RBI = ival; break;
				case  28: sscanf (here, "%d", &ival); GLend->log.Visit.OffenseStats.SH = ival; break;
				case  29: sscanf (here, "%d", &ival); GLend->log.Visit.OffenseStats.SF = ival; break;
				case  30: sscanf (here, "%d", &ival); GLend->log.Visit.OffenseStats.HBP = ival; break;
				case  31: sscanf (here, "%d", &ival); GLend->log.Visit.OffenseStats.BB = ival; break;
				case  32: sscanf (here, "%d", &ival); GLend->log.Visit.OffenseStats.IBB = ival; break;
				case  33: sscanf (here, "%d", &ival); GLend->log.Visit.OffenseStats.K = ival; break;
				case  34: sscanf (here, "%d", &ival); GLend->log.Visit.OffenseStats.SB = ival; break;
				case  35: sscanf (here, "%d", &ival); GLend->log.Visit.OffenseStats.CS = ival; break;
				case  36: sscanf (here, "%d", &ival); GLend->log.Visit.OffenseStats.GDP = ival; break;
				case  37: sscanf (here, "%d", &ival); GLend->log.Visit.OffenseStats.CI = ival; break;
				case  38: sscanf (here, "%d", &ival); GLend->log.Visit.OffenseStats.LOB = ival; break;
				case  39: sscanf (here, "%d", &ival); GLend->log.Visit.PitchStats.PitchersUsed = ival; break;
				case  40: sscanf (here, "%d", &ival); GLend->log.Visit.PitchStats.PitchERs = ival; break;
				case  41: sscanf (here, "%d", &ival); GLend->log.Visit.PitchStats.TeamERs = ival; break;
				case  42: sscanf (here, "%d", &ival); GLend->log.Visit.PitchStats.WP = ival; break;
				case  43: sscanf (here, "%d", &ival); GLend->log.Visit.PitchStats.BK = ival; break;
				case  44: sscanf (here, "%d", &ival); GLend->log.Visit.DefenseStats.Putouts = ival; break;
				case  45: sscanf (here, "%d", &ival); GLend->log.Visit.DefenseStats.Assists = ival; break;
				case  46: sscanf (here, "%d", &ival); GLend->log.Visit.DefenseStats.E = ival; break;
				case  47: sscanf (here, "%d", &ival); GLend->log.Visit.DefenseStats.PB = ival; break;
				case  48: sscanf (here, "%d", &ival); GLend->log.Visit.DefenseStats.DP = ival; break;
				case  49: sscanf (here, "%d", &ival); GLend->log.Visit.DefenseStats.TP = ival; break;
				case  50: sscanf (here, "%d", &ival); GLend->log.Home.OffenseStats.AB = ival; break;
				case  51: sscanf (here, "%d", &ival); GLend->log.Home.OffenseStats.H = ival; break;
				case  52: sscanf (here, "%d", &ival); GLend->log.Home.OffenseStats.B2 = ival; break;
				case  53: sscanf (here, "%d", &ival); GLend->log.Home.OffenseStats.B3 = ival; break;
				case  54: sscanf (here, "%d", &ival); GLend->log.Home.OffenseStats.HR = ival; break;
				case  55: sscanf (here, "%d", &ival); GLend->log.Home.OffenseStats.RBI = ival; break;
				case  56: sscanf (here, "%d", &ival); GLend->log.Home.OffenseStats.SH = ival; break;
				case  57: sscanf (here, "%d", &ival); GLend->log.Home.OffenseStats.SF = ival; break;
				case  58: sscanf (here, "%d", &ival); GLend->log.Home.OffenseStats.HBP = ival; break;
				case  59: sscanf (here, "%d", &ival); GLend->log.Home.OffenseStats.BB = ival; break;
				case  60: sscanf (here, "%d", &ival); GLend->log.Home.OffenseStats.IBB = ival; break;
				case  61: sscanf (here, "%d", &ival); GLend->log.Home.OffenseStats.K = ival; break;
				case  62: sscanf (here, "%d", &ival); GLend->log.Home.OffenseStats.SB = ival; break;
				case  63: sscanf (here, "%d", &ival); GLend->log.Home.OffenseStats.CS = ival; break;
				case  64: sscanf (here, "%d", &ival); GLend->log.Home.OffenseStats.GDP = ival; break;
				case  65: sscanf (here, "%d", &ival); GLend->log.Home.OffenseStats.CI = ival; break;
				case  66: sscanf (here, "%d", &ival); GLend->log.Home.OffenseStats.LOB = ival; break;
				case  67: sscanf (here, "%d", &ival); GLend->log.Home.PitchStats.PitchersUsed = ival; break;
				case  68: sscanf (here, "%d", &ival); GLend->log.Home.PitchStats.PitchERs = ival; break;
				case  69: sscanf (here, "%d", &ival); GLend->log.Home.PitchStats.TeamERs = ival; break;
				case  70: sscanf (here, "%d", &ival); GLend->log.Home.PitchStats.WP = ival; break;
				case  71: sscanf (here, "%d", &ival); GLend->log.Home.PitchStats.BK = ival; break;
				case  72: sscanf (here, "%d", &ival); GLend->log.Home.DefenseStats.Putouts = ival; break;
				case  73: sscanf (here, "%d", &ival); GLend->log.Home.DefenseStats.Assists = ival; break;
				case  74: sscanf (here, "%d", &ival); GLend->log.Home.DefenseStats.E = ival; break;
				case  75: sscanf (here, "%d", &ival); GLend->log.Home.DefenseStats.PB = ival; break;
				case  76: sscanf (here, "%d", &ival); GLend->log.Home.DefenseStats.DP = ival; break;
				case  77: sscanf (here, "%d", &ival); GLend->log.Home.DefenseStats.TP = ival; break;
				case  78: strcpy(GLend->log.UmpireHome.ID, here); break;
				case  79: break;
				case  80: strcpy(GLend->log.Umpire1B.ID, here); break;
				case  81: break;
				case  82: strcpy(GLend->log.Umpire2B.ID, here); break;
				case  83: break;
				case  84: strcpy(GLend->log.Umpire3B.ID, here); break;
				case  85: break;
				case  86: strcpy(GLend->log.UmpireLF.ID, here); break;
				case  87: break;
				case  88: strcpy(GLend->log.UmpireRF.ID, here); break;
				case  89: break;
				case  90: strcpy(GLend->log.Visit.Manager.ID, here); break;
				case  91: break;
				case  92: strcpy(GLend->log.Home.Manager.ID, here); break;
				case  93: break;
				case  94: strcpy(GLend->log.WP.ID, here); break;
				case  95: break;
				case  96: strcpy(GLend->log.LP.ID, here); break;
				case  97: break;
				case  98: strcpy(GLend->log.SaveP.ID, here); break;
				case  99: break;
				case 100: strcpy(GLend->log.GWRBI.ID, here); break;
				case 101: break;
				case 102: strcpy(GLend->log.Visit.SP.ID, here); break;
				case 103: break;
				case 104: strcpy(GLend->log.Home.SP.ID, here); break;
				case 105: break;
				case 106:
				case 109:
				case 112:
				case 115:
				case 118:
				case 121:
				case 124:
				case 127:
				case 130: strcpy(GLend->log.Visit.Start[(i-106)/3].IDName.ID, here); break;
				case 107:
				case 110:
				case 113:
				case 116:
				case 119:
				case 122:
				case 125:
				case 128:
				case 131: break;
				case 108:
				case 111:
				case 114:
				case 117:
				case 120:
				case 123:
				case 126:
				case 129:
				case 132: sscanf (here, "%d", &ival); GLend->log.Visit.Start[(i-106)/3].DefensePos = ival; break;
				case 133:
				case 136:
				case 139:
				case 142:
				case 145:
				case 148:
				case 151:
				case 154:
				case 157: strcpy(GLend->log.Home.Start[(i-133)/3].IDName.ID, here); break;
				case 134:
				case 137:
				case 140:
				case 143:
				case 146:
				case 149:
				case 152:
				case 155:
				case 158: break;
				case 135:
				case 138:
				case 141:
				case 144:
				case 147:
				case 150:
				case 153:
				case 156:
				case 159: sscanf (here, "%d", &ival); GLend->log.Home.Start[(i-133)/3].DefensePos = ival; break;
				case 160: break; /* TODO */
				case 161: GLend->log.AcquisitionInfo = *here; break;
			}
		}
	}
	(void) fclose(fp);
}

void GLFile_Close (void)
{
	struct GameLogType *n;
	for (struct GameLogType *p = GLstart; p; p = n)
	{
		n = p->next;
		free(p);
	}
}

void GLFile_DumpCompletion (struct GLCompletion* p)
{
   printf ("Completion\n");
   printf ("Date = %s\n", p->Date);
   printf ("Park = %s\n", p->Park);
   printf ("vs = %d\n", p->vs);
   printf ("hs = %d\n", p->hs);
   printf ("len = %d\n", p->len);
};

void GLFile_DumpIDName (char* lab, struct GLIDNameType* p)
{
	printf ("%s ID = %s", lab, p->ID);
}
void GLFile_DumpStart (struct GLStartType* p)
{
	char s[4];
	for (int i=0; i<NUM_STARTS; i++)
	{
		sprintf (s, "[%d]", i);
		GLFile_DumpIDName (s, &p[i].IDName);
		printf ("\tDefensePos = %d\n", p[i].DefensePos);
	}
}
void GLFile_DumpOffenseStats (struct GLOffenseStats* p)
{
   printf ("AB = %d\n", p->AB);
   printf ("H = %d\n", p->H);
   printf ("B2 = %d\n", p->B2);
   printf ("B3 = %d\n", p->B3);
   printf ("HR = %d\n", p->HR);
   printf ("RBI = %d\n", p->RBI);
   printf ("SH = %d\n", p->SH);
   printf ("SF = %d\n", p->SF);
   printf ("HBP = %d\n", p->HBP);
   printf ("BB = %d\n", p->BB);
   printf ("IBB = %d\n", p->IBB);
   printf ("K = %d\n", p->K);
   printf ("SB = %d\n", p->SB);
   printf ("CS = %d\n", p->CS);
   printf ("GDP = %d\n", p->GDP);
   printf ("CI = %d\n", p->CI);
   printf ("LOB = %d\n", p->LOB);
}
void GLFile_DumpDefenseStats (struct GLDefenseStats* p)
{
   printf ("Putouts = %d\n", p->Putouts);
   printf ("Assists = %d\n", p->Assists);
   printf ("E = %d\n", p->E);
   printf ("PB = %d\n", p->PB);
   printf ("DP = %d\n", p->DP);
   printf ("TP = %d\n", p->TP);
}
void GLFile_DumpPitchStats (struct GLPitchStats* p)
{
   printf ("PitchersUsed = %d\n", p->PitchersUsed);
   printf ("PitchERs = %d\n", p->PitchERs);
   printf ("TeamERs = %d\n", p->TeamERs);
   printf ("WP = %d\n", p->WP);
   printf ("BK = %d\n", p->BK);
}
void GLFile_DumpTeam (char* lab, struct GLTeamDataType* p)
{
   printf ("%s\n", lab);
   printf ("Team = %s\n", p->Team);
   printf ("League = %s\n", p->League);
   printf ("TeamGameNum = %d\n", p->TeamGameNum);
   printf ("TeamScore = %d\n", p->TeamScore);
   printf ("LineScore = %s\n", p->LineScore);
   GLFile_DumpOffenseStats (&p->OffenseStats);
   GLFile_DumpPitchStats (&p->PitchStats);
   GLFile_DumpDefenseStats (&p->DefenseStats);
   GLFile_DumpIDName ("Manager", &p->Manager); printf ("\n");
   GLFile_DumpIDName ("SP", &p->SP); printf ("\n");
   GLFile_DumpStart (p->Start);
}
void GLFile_Dump (void)
{
	for (struct GameLogType *p = GLstart; p; p = p->next)
	{
		GLFile_DumpGame(&p->log);
	}
}

struct GLFileType* GLFile_GetGame(char* GameID) /* 8: date, 3: home, 1: NumofGame */
{
	struct GLFileType* retval = 0;
	for (struct GameLogType *n = GLstart; n && !retval; n = n->next)
	{
		if (!strncmp(GameID, n->log.Date, DATE_SIZE-1) &&
		    !strncmp(GameID+DATE_SIZE-1, n->log.Home.Team, TEAM_SIZE-1) &&
		    n->log.NumGames == GameID[DATE_SIZE+TEAM_SIZE-2])
		{
			retval = &n->log;
		}
	}
	return retval;
}

void GLFile_DumpGame(struct GLFileType* p)
{
	printf ("Date = %s\n", p->Date);
	printf ("NumGames = %c\n", p->NumGames);
	printf ("DayOfWeek = %s\n", p->DayOfWeek);
	printf ("DayNight = %c\n", p->DayNight);
	GLFile_DumpCompletion (&p->Completion);
	printf ("ForfeitInfo = %c\n", p->ForfeitInfo);
	printf ("ProtestInfo = %s\n", p->ProtestInfo);
	printf ("ParkID = %s\n", p->ParkID);
	printf ("Attendance = %d\n", p->Attendance);
	printf ("TimeOfGame = %d\n", p->TimeOfGame);
	GLFile_DumpIDName ("UmpireHome", &p->UmpireHome); printf ("\n");
	GLFile_DumpIDName ("Umpire1B", &p->Umpire1B); printf ("\n");
	GLFile_DumpIDName ("Umpire2B", &p->Umpire2B); printf ("\n");
	GLFile_DumpIDName ("Umpire3B", &p->Umpire3B); printf ("\n");
	GLFile_DumpIDName ("UmpireLF", &p->UmpireLF); printf ("\n");
	GLFile_DumpIDName ("UmpireRF", &p->UmpireRF); printf ("\n");
	GLFile_DumpIDName ("WP", &p->WP); printf ("\n");
	GLFile_DumpIDName ("LP", &p->LP); printf ("\n");
	GLFile_DumpIDName ("SaveP", &p->SaveP); printf ("\n");
	GLFile_DumpIDName ("GWRBI", &p->GWRBI); printf ("\n");
	//printf ("AdditionalInfo = %s\n", p->AdditionalInfo);
	printf ("AcquisitionInfo = %c\n", p->AcquisitionInfo);
	GLFile_DumpTeam ("Visit", &p->Visit);
	GLFile_DumpTeam ("Home", &p->Home);
}