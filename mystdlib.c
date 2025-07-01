#include "mystdlib.h"
#undef malloc
#undef free
#include <stdlib.h>

int mallocctr = 0;
int freectr = 0;
void *mymalloc(size_t size)
{
    mallocctr++;
    return malloc(size);
}

void myfree(void *ptr)
{
    freectr++;
    free(ptr);
}