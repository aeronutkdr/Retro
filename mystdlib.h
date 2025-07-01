#ifndef _MYMALLOC_H_
#define _MYMALLOC_H_
#include <stddef.h>

extern int mallocctr;
extern int freectr;
void *mymalloc(size_t size);
void myfree(void *ptr);

#define malloc mymalloc
#define free myfree

#endif /* _MYMALLOC_H_ */