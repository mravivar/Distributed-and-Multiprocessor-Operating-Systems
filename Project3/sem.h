#include <stdio.h>
#include <unistd.h>
#include "threads.h"

struct semaphore_t{
    struct TCB_t *queue;
    int counter;
};

struct semaphore_t * CreateSem(int inputValue){
   struct semaphore_t *sem = (struct semaphore_t*) malloc(sizeof(struct semaphore_t));
   sem->queue = newQueue();
   sem->counter = inputValue;
}

void P(struct semaphore_t *sem){
    sem->counter = sem->counter - 1;
    if(sem->counter < 0){
        struct TCB_t *current_thread = getCurrentThread();
        AddQueue(sem->queue, current_thread);
	switchContext();
    }
}

void V(struct semaphore_t *sem){
   sem->counter = sem->counter + 1;
   if(sem->counter <= 0){
       struct TCB_t *thread = DelQueue(sem->queue);
       addToReadyQueue(thread);
       yield();      
   }
}
