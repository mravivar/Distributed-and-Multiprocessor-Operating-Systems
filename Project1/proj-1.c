#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "sem.h"

//Author
//Vivek Narsimha Dondeti
//Murali Ravivarma

struct arg_struct{
  int array_pointer;
  semaphore_t* start_semaphore;
  semaphore_t* end_semaphore;
};

semaphore_t mainthread, thread1, thread2, thread3;
int array[3];

void increment_function(struct arg_struct *arg){
  while(1){
    P(arg->start_semaphore);
    array[arg->array_pointer]++;
    V(arg->end_semaphore);
    sleep(1);
  }
}

int main(){
  for(int i = 0; i < 3; i++){
    array[i] = 0;
  }
  init_sem(&mainthread, 0);
  init_sem(&thread1, 1);
  init_sem(&thread2, 0);
  init_sem(&thread3, 0);

  struct arg_struct arg1;
  arg1.array_pointer = 0;
  arg1.start_semaphore = &thread1;
  arg1.end_semaphore = &thread2;
  start_thread(increment_function, &arg1);

  struct arg_struct arg2;
  arg2.array_pointer = 1;
  arg2.start_semaphore = &thread2;
  arg2.end_semaphore = &thread3;
  start_thread(increment_function, &arg2);

  struct arg_struct arg3;
  arg3.array_pointer = 2;
  arg3.start_semaphore = &thread3;
  arg3.end_semaphore = &mainthread;
  start_thread(increment_function, &arg3);

  while(1){
    P(&mainthread);
    printf("The value at index 0 is %d\nThe value at index 1 is %d\nThe value at index 2 is %d\n", array[0], array[1], array[2]);
    V(&thread1);
  }
}
