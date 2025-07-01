#ifndef _EVLADJ_H_
#define _EVLADJ_H_

struct LadjType
{
    int Home;
    int Position;
};

char ProcessLadj(char* str, struct LadjType* pladj);

#endif /* _EVLADJ_H_ */
