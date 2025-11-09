#include <assert.h> /* for assert() */
#include <dirent.h>
#include <stdlib.h>
#include "evParse.h"
#include "GJ.h"
#include "State.h"
#include "StateSolve.h"
#include "..\EventFields.h"

#ifdef DEBUG
#include <string.h>
#define DEBUG_PRINT() (void) fprintf (stdout, "**** %d %d ****\n", malloc_ct, malloc_sz);
#else
#define DEBUG_PRINT()
#endif

#define LINELEN (1023)
#define NUMFIELDS (97)
#define NUMSTATES (40)

int main (int argc, char* argv[])
{
#if 0
	double mat[] = {1.,1.,19020.,16.,12.,250172.};
	int flag = PerformOperation(mat, 2);
	if (flag == 1) flag = CheckConsistency(mat, 2, flag);
	PrintMatrix(mat, 2);
	PrintResult(mat, 2, flag);
	/*
	double a[] = { 0, 2, 1, 4 , 
				   1, 1, 2, 6 , 
				   2, 1, 1, 7 };
	int flag = PerformOperation(a, 3);
	if (flag == 1) flag = CheckConsistency(a, 3, flag);
	PrintMatrix(a, 3);
	PrintResult(a, 3, flag);
	*/
#endif
	char str[LINELEN+1];
	char* fields[NUMFIELDS];
	unsigned short states[NUMSTATES] = {0};
	struct StateType* root = 0;
	int lineNum = 0;
    DIR *dir;
    struct dirent *dent;
    dir = opendir(argv[1]);
    assert (dir);
	int dir_len = strlen(argv[1]);
	while((dent=readdir(dir))!=NULL)
	{
		char* ptxt = strstr(dent->d_name, ".txt");
		if (ptxt && !ptxt[4])
		{
			char* fullpath = malloc (dir_len +
			                         strlen(dent->d_name) + 2);
			strcpy(fullpath, argv[1]);
			fullpath[dir_len] = '\\';
			strcpy(fullpath+dir_len+1, dent->d_name);
			FILE* fp = fopen(fullpath, "r");
			free(fullpath);
			states[0] = 0xFFFF;
			while (fgets(str, LINELEN, fp))
			{
				lineNum++;
				assert (strlen(str) <= LINELEN);
				Parse (fields, str);
				/*
				if (!strcmp(fields[inning], "10"))
				{
					printf("%d\n", lineNum);
				}
				*/
				enum EventType Event;
				int isFFFF = (states[0] == 0xFFFF);
				int n = Process(states, fields, &Event);
				root = State_Sequence(root, states, n, isFFFF);
				states[0] = states[n-1];
			}
			fclose(fp);
		}
	}
	assert (!closedir(dir));
	State_SetOrder(root);
	DEBUG_PRINT();
/**/
	unsigned int sz = State_Num(root);
	double *matrix = malloc (sz * (sz+1) * sizeof(double));
	SS_Solve(root, matrix, sz);
	SS_DumpMatrix(root, matrix, sz);
	int flag = PerformOperation(matrix, sz);
	if (flag == 1) flag = CheckConsistency(matrix, sz, flag);
	SS_Final(root, matrix, sz, flag);
	State_UpdateScores(root, matrix, sz);
	free(matrix);
/**/
	State_DumpRoot(root);
	char* fullpath = malloc (dir_len + 12);
	strcpy(fullpath, argv[1]);
	strcat(fullpath, "\\States.bin");
	FILE* fp = fopen(fullpath, "wb");
	State_DumpRootBin(fp, root);
	fclose(fp);
	free(fullpath);
/**/
	State_FreeRoot(root);
	DEBUG_PRINT();
	return 0;
}