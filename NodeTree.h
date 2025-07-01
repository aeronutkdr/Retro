#ifndef _NODETREE_H_
#define _NODETREE_H_
void NT_AddRoute(int from, int to, unsigned short freq);
int NT_Dump(void);
void NT_Drop(void);
void NT_DumpBin(char* fname);
void NT_DumpBin2(char* fname);
void NT_DumpBin3(char* fname);
int* NT_CalcFreqs(int* c);
void NT_LoadBin(char* fname);
unsigned int NT_Aggregate(unsigned char from, unsigned int* agg);

struct NT_SolveType
{
	unsigned short state;
	double value;
};
struct NT_SolveType* NT_Solve(int* n);

#endif /* _NODETREE_H_ */