#include "q.h"
#include <stdlib.h>

struct TCB_t *ReadyQ; // points to a queue of TCBs
struct TCB_t *Curr_Thread; // points to the thread under execution (oneTCB)
int thread_count=0;

void start_thread(void (*function)(void))
{
    /*
    allocate a stack (via malloc) of a certain size (choose 8192)
    allocate a TCB (via malloc)
    call init_TCB with appropriate arguments
    Add a thread_id (use a counter)
    call addQ to add this TCB into the “ReadyQ” which is a global header pointer
    */
    void *stack;
    stack = malloc(8192);
    struct TCB_t *TCB;
    TCB = (struct TCB_t *)malloc(sizeof(struct TCB_t));
    init_TCB(TCB, function, stack, sizeof(stack));
    TCB->thread_id=++thread_count; //Add a thread_id (use a counter)
    AddQueue(ReadyQ, TCB);
}

void run() {
    // real code

    Curr_Thread = DelQueue(ReadyQ);
    ucontext_t parent;      // get a place to store the main context, for faking
    getcontext(&parent);    // magic sauce
    swapcontext(&parent, &(Curr_Thread->context)); // start the first thread
}

void yield() { // similar to run
    struct TCB_t *Prev_Thread;
    AddQueue(ReadyQ, Curr_Thread);
    Prev_Thread = Curr_Thread;
    Curr_Thread = DelQueue(ReadyQ);
    swapcontext(&(Prev_Thread->context), &(Curr_Thread->context)); //swap the context, from Prev_Thread to the thread pointed to Curr_Thread
}

//You may want to add a function to print the thread_id, given a pointer of type TCB_t.
/*
void printThreadID(struct TCB_t *item) {
    printf("thread_id: %d\n", item->thread_id);
}
*/
