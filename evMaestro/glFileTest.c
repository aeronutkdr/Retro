#include "GLFile.h"

int main (int argc, char* argv[])
{
	GLFile_Open(argv[1]);
	GLFile_Dump();
	struct GLFileType* g = GLFile_GetGame("20220511MIN0");
	if (g) GLFile_DumpGame(g);
	GLFile_Close();
	return 0;
}
