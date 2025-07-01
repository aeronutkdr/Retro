#include "State.h"
#include <assert.h> /* for assert() */

#ifdef DEBUG
#include <stdio.h>
#define DEBUG_PRINT() (void) printf ("**** %d %d ****\n", malloc_ct, malloc_sz);
#else
#define DEBUG_PRINT()
#endif

int main(int argc, char* argv[])
{
    struct StateType a = {.freq = 0, .to0 = 0, .value = 0};
    struct StateType b = {.freq = 0, .to0 = 0, .value = 1};
    struct StateType c = {.freq = 0, .to0 = 0, .value = 2};
    struct StateType* root  = 0;
    unsigned short states1[] = {0,1,2,0,2,1,2};
    unsigned short states2[] = {0x0000,
                                0x0480,
                                0x0481,
                                0x0501,
                                0x1000};
    goto Test4;

Test1:
    /* 012 */
    a.freq++; State_Transition(&a, &b);
    State_Transition(&b, &c);
    State_Dump(&a);
    State_Dump(&b);
    State_Dump(&c);
    DEBUG_PRINT();
    /* 02 */
    a.freq++; State_Transition(&a, &c);
    State_Dump(&a);
    State_Dump(&b);
    State_Dump(&c);
    DEBUG_PRINT();
    /* 12 */
    b.freq++; State_Transition(&b, &c);
    State_Dump(&a);
    State_Dump(&b);
    State_Dump(&c);
    DEBUG_PRINT();
    /* 012 */
    a.freq++; State_Transition(&a, &b);
    State_Transition(&b, &c);
    State_Dump(&a);
    State_Dump(&b);
    State_Dump(&c);
    DEBUG_PRINT();

    State_Free(&a);
    DEBUG_PRINT();
    State_Free(&b);
    DEBUG_PRINT();
    State_Free(&c);
    DEBUG_PRINT();
    assert (!malloc_ct);
    malloc_sz = 0;
    goto eof;
    
Test2:
    root = State_Sequence(root, states1+0, 3);
    State_DumpRoot(root);
    DEBUG_PRINT();
    root = State_Sequence(root, states1+3, 2);
    DEBUG_PRINT();
    root = State_Sequence(root, states1+5, 2);
    DEBUG_PRINT();
    root = State_Sequence(root, states1+0, 3);
    DEBUG_PRINT();
    State_DumpRoot(root);
    State_FreeRoot(root);
    DEBUG_PRINT();
    assert (!malloc_ct);
    malloc_sz = 0;
    goto eof;

Test3:
    root = State_Sequence(0, states2, 5);
    State_DumpRoot(root);
    DEBUG_PRINT();
    State_FreeRoot(root);
    DEBUG_PRINT();
    assert (!malloc_ct);
    malloc_sz = 0;
    goto eof;

Test4:
    /* swap states */
    states2[0] = 0x0480;
    states2[1] = 0x0000;
    root = State_Sequence(0, states2, 5);
    State_SetOrder(root);
    State_DumpRoot(root);
    DEBUG_PRINT();
    State_FreeRoot(root);
    DEBUG_PRINT();
    assert (!malloc_ct);
    goto eof;

eof:
    return 0;
}