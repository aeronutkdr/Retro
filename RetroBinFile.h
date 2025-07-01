#ifndef _RETROBINFILE_H_
#define _RETROBINFILE_H_
struct State
{
	unsigned short State;
	unsigned short NumTos;
	unsigned int   Value;
	unsigned int   FreqIn;
};
struct To
{
    struct State* State;
    unsigned int  Freq;
};

void RBFile_Open(char* fname);
struct State* RBFile_GetState(unsigned short s);
struct State* RBFile_GetStateByIndex(int i);
struct To* RBFile_GetNext(struct State* s, int i);
void RBFile_Close(void);

#endif /* _RETROBINFILE_H_ */