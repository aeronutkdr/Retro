#ifndef _STATESOLVE_H_
#define _STATESOLVE_H_
#include "State.h"

unsigned int SS_Solve     (struct StateType* root,
                           double*           matrix,
                           unsigned int      n);
void         SS_DumpMatrix(struct StateType* root,
                           double*           matrix,
                           unsigned int      n);
void         SS_Final     (struct StateType* root,
                           double*           matrix,
                           unsigned int      n,
                           unsigned int      flag);

#endif /* _STATESOLVE_H_ */