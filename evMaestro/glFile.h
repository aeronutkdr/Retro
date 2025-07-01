#ifndef _GLFILE_H_
#define _GLFILE_H_
#include "glFileTypes.h"
/* https://www.retrosheet.org/gamelogs/glfields.txt */

void GLFile_Open (char* name);
void GLFile_Close (void);
void GLFile_Dump (void);
struct GLFileType* GLFile_GetGame(char* GameID); /* 8: date, 3: home, 1: NumofGame */
void GLFile_DumpGame(struct GLFileType* p);

#endif /* _GLFILE_H_ */
