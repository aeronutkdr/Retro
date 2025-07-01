#include "mystdlib.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "GJ.h"
#include "NodeTree.h"
static struct Node
{
	int          val;
	struct Node* pNext;
	struct Node* pState;
	int freq;
} *start = 0;
static int NT_DumpNext(struct Node *n);
static int NT_DumpNexts(struct Node* n);
static int NT_DumpState(struct Node *s);
static struct Node* NT_GetState(int val);
static int NT_IncPath(struct Node* fNode, struct Node* tNode, unsigned short freq);

static int MaxRoutes = 0;
static int NumStates = 0;
#define MAX(a,b) (((a)>(b))?(a):(b))
void NT_AddRoute(int from, int to, unsigned short freq)
{
	/* get from node */
	struct Node* fNode = NT_GetState(from);
	if (!start)
		start = fNode;
	if (start->val > fNode->val)
		start = fNode;
	/* get to node */
	struct Node* tNode = NT_GetState(to);
	if (start->val > tNode->val)
		start = tNode;
	fNode->freq += NT_IncPath(fNode, tNode, freq);
	MaxRoutes = MAX(MaxRoutes, fNode->freq);
}
int NT_Dump(void)
{
	int retval = 0;
	for (struct Node* p = start; p; p = p->pState)
	{
		retval += NT_DumpState(p);
	}
	return retval;
}
void NT_LoadBin(char* fname)
{
	FILE* fp = fopen(fname, "rb");
	unsigned int rVal;
	while (fread(&rVal, sizeof(int), 1, fp))
	{
		unsigned short fval = rVal & 0xFFFF;
		unsigned short freq = rVal >> 16;
		for (int i=0; i<freq; i++)
		{
			fread(&rVal, sizeof(int), 1, fp);
			unsigned short tval = rVal & 0xFFFF;
			unsigned short f2 = rVal >> 16;
			NT_AddRoute(fval, tval, f2);
		}
	}
	fclose(fp);
}
struct BinStateType
{
	unsigned short State;
	unsigned short Pad;
	unsigned int   Freq;
};
struct BinType
{
	unsigned short State;
	unsigned short NumTos;
	unsigned int   Value;
	unsigned int   FreqIn;
};
void NT_DumpBin2(char* fname)
{
	int num;
	int* f = NT_CalcFreqs(&num);
	struct NT_SolveType* solve = NT_Solve(&num);
	assert (num == NumStates);
	struct Node* p;
	int i;
	FILE* fp = fopen(fname, "wb");
	for (i=0, p=start; i<num; i++, p = p->pState)
	{
		if (p->val == solve[i].state);
		else
		{
			assert (0);
		}
		unsigned short numTo = p->freq;
		int length = sizeof(struct BinType) + numTo*sizeof(struct BinStateType);
		struct BinType *out = malloc(length);
		struct BinStateType* to = (struct BinStateType*) (out + 1);
		out->State = p->val;
		out->NumTos = numTo;
		out->Value = (unsigned int)(solve[i].value * (double)(1<<29));
		out->FreqIn = f[p->val];
		for (struct Node* n = p->pNext; n; n = n->pNext, to++)
		{
			to->State = n->pState->val;
			to->Pad = 0;
			to->Freq = n->val;
		}
		fwrite (out, length, 1, fp);
		free(out);
	}
	free(solve);
	free(f);
	fclose(fp);
}
struct Bin3ToType
{
	unsigned int   Freq;
	unsigned short State;
	unsigned short Pad;
};
struct Bin3StateType
{
	unsigned int   FreqIn;
	unsigned int   Value;
	unsigned short NumTos;
	unsigned short Pad;
};
void NT_DumpBin3(char* fname)
{
	unsigned int TOC[0x4000] = {0};
	struct Node* p;
	unsigned int cur = 0x4000 << 2;
	for (struct Node* p = start; p; p = p->pState)
	{
		TOC[p->val] = cur;
		cur += (12 + 8 * p->freq);
	}
	FILE* fp = fopen(fname, "wb");
	fwrite (TOC, 4, 0x4000, fp);
	int num;
	int* f = NT_CalcFreqs(&num);
	struct NT_SolveType* solve = NT_Solve(&num);
	assert (num == NumStates);
	int i;
	for (i=0, p=start; i<num; i++, p = p->pState)
	{
		if (p->val == solve[i].state);
		else
		{
			assert (0);
		}
		unsigned short numTo = p->freq;
		int length = sizeof(struct Bin3StateType) + numTo*sizeof(struct Bin3ToType);
		struct Bin3StateType *out = malloc(length);
		struct Bin3ToType* to = (struct Bin3ToType*) (out + 1);
		out->NumTos = numTo;
		out->Value = (unsigned int)(solve[i].value * (double)(1<<29));
		out->FreqIn = f[p->val];
		out->Pad = 0;
		for (struct Node* n = p->pNext; n; n = n->pNext, to++)
		{
			to->State = n->pState->val;
			to->Pad = 0;
			to->Freq = n->val;
		}
		fwrite (out, length, 1, fp);
		free(out);
	}
	free(solve);
	free(f);
	fclose(fp);
}
void NT_DumpBin(char* fname)
{
	FILE* fp = fopen(fname, "wb");
	for (struct Node* p = start; p; p = p->pState)
	{
		fwrite (&p->val,2,1,fp);
		fwrite (&p->freq,2,1,fp);
		int count;
		struct Node* q;
		for (q = p->pNext, count = 0; q; q = q->pNext, count++)
		{
			fwrite(&q->pState->val,1,2,fp);
			fwrite(&q->val,1,2,fp);
		}
		/*
		for (;count<MaxRoutes;count++)
		{
			fwrite((void*)"\x00\x00\x00\x00", 1, 4, fp);
		}
		*/
	}
	fclose(fp);
}
void NT_Drop(void)
{
	struct Node* pState = 0;
	for (struct Node* p = start; p; p = pState)
	{
		pState = p->pState;
		struct Node* pNext = 0;
		for (struct Node *n = p->pNext; n; n = pNext)
		{
			pNext = n->pNext;
			free(n);
			p->freq--;
		}
		assert(p->freq == 0);
		free(p);
	}
}
static int NT_DumpNext(struct Node *n)
{
	printf ("%04X: %d\t", n->pState->val, n->val);
	return n->val;
}
static int NT_DumpNexts(struct Node* n)
{
	int retval = 0;
	for (;n;n = n->pNext)
	{
		retval += NT_DumpNext(n);
	}
	printf("\n");
	return retval;
}
static int NT_DumpState(struct Node *s)
{
	int retval = 0;
	/*if (s->pNext)*/
	{
		printf ("val = %04X,%d:\t", s->val, s->freq);
		retval += NT_DumpNexts(s->pNext);
	}
	return retval;
}
#define MAX_STATE (0x4000)
static struct Node* NT_GetState(int val)
{
	struct Node* p = start;
	struct Node* p_prev = 0;
	while (p && p->val < val)
	{
		p_prev = p;
		p = p->pState;
	}
	if (!p || (p->val != val))
	{
		if (val < MAX_STATE) NumStates++;
		struct Node* p_next = p;
		p = malloc(sizeof(struct Node));
		p->val = val;
		p->pNext = 0;
		p->pState = p_next;
		p->freq = 0;
		if (p_prev) p_prev->pState = p;
	}
	return p;
}
static int NT_IncPath(struct Node* fNode, struct Node* tNode, unsigned short freq)
{
	struct Node* p;
	struct Node* p_prev = 0;
	int retval = 0;
	for (p = fNode->pNext; p && (tNode->val > p->pState->val); p = p->pNext)
	{
		p_prev = p;
	}
	if (!p || (tNode->val != p->pState->val))
	{
		retval = 1;
		struct Node* n = malloc (sizeof(struct Node));
		n->val = 0;
		n->pState = tNode;
		n->pNext = p;
		if (p_prev) p_prev->pNext = n;
		else        fNode->pNext = n;
		p = n;
	}
	p->val+=freq;
	return retval;
}

int* NT_CalcFreqs(int* c)
{
	(*c)=0;
	for (struct Node* p = start; p; p = p->pState)
	{
		(*c) = p->val;
	}
	(*c)++;
	int* retval = malloc(2*(*c)*sizeof(int));
	memset(retval, 0, (2*(*c)*sizeof(int)));
	for (struct Node* p = start; p; p = p->pState)
	{
		for (struct Node* q = p->pNext; q; q = q->pNext)
		{
			if (p->val == 0)
			{
				//printf ("stop\n");
			}
			retval[(*c)+p->val] += q->val;
			retval[q->pState->val] += q->val;
		}
	}
	return retval;
}

int NT_RunValue(unsigned short st)
{
	return ((st>>(0+8))&1) +
	       ((st>>(1+8))&1) +
	       ((st>>(2+8))&1) +
	       ((st>>(3+8))&1) +
	       ((st>>(4+8))&3);
}

int NT_IndexOf(struct Node* s)
{
	struct Node* p = start;
	int i=0;
	for (; p && (p != s); p = p->pState, i++);
	return i;
}

struct NT_SolveType* NT_Solve(int* n)
{
	*n = NumStates;
	double* mat = malloc(sizeof(double)*NumStates*(NumStates+1));
	struct Node* p = start;
	int i=0;
	for (; p && (p->val < MAX_STATE); p = p->pState, i++)
	{
		int Sum = 0;
		struct Node* n = p->pNext;
		for (; n && (n->pState->val < MAX_STATE); n = n->pNext)
		{
			Sum += n->val;
			int idx = NT_IndexOf(n->pState);
			mat[i*(NumStates+1)+idx] = n->val;
			mat[i*(NumStates+1)+NumStates] += n->val * NT_RunValue(n->pState->val);
		}
		mat[i*(NumStates+1)+i] -= MAX(Sum,1);
		mat[i*(NumStates+1)+NumStates] -= Sum*NT_RunValue(p->val | 0x0100);
		if (p->val >= 0x3000)
			mat[i*(NumStates+1)+NumStates] = 0;
	}
	//PrintMatrix(mat, NumStates);
	int flag = PerformOperation(mat, NumStates);

	if (flag == 1)
		flag = CheckConsistency(mat, NumStates, flag);

	double* d = BuildResults(mat, NumStates, flag);
	struct NT_SolveType* retval = 0;
	if (d)
	{
		retval = malloc(sizeof(struct NT_SolveType)*NumStates);
		for (p = start, i=0; p; p = p->pState, i++)
		{
			retval[i].state = p->val;
			retval[i].value = d[i];
		}
		free (d);
	}
	free(mat);
	return retval;
}

unsigned int NT_Aggregate(unsigned char from, unsigned int* agg)
{
	unsigned short f = from << 8;
	unsigned int retval = 0;
	struct Node *p0;
	for (p0 = start; p0 && (p0->val != f); p0 = p0->pState);
	if (!p0) return retval;
	struct Node *n, *p;
	for (p = p0; p && ((p->val & 0xFF00) == f); p = p->pState)
	{
		for (n = p->pNext; n; n = n->pNext)
		{
			if ((n->pState->val & 0xFF) == 0)
			{
				int v = n->pState->val >> 8;
				agg[v] += n->val;
				retval += n->val;
			}
		}
	}
	return retval;
}