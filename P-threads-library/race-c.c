// (c) Partha Dasgupta 2009
// permission to use and distribute granted.

#include <sched.h>
#include <stdio.h>
#include <stdlib.h>

void *stack;
int x=1;

func1()
{int i = 0;
   printf("Thread 1 - running\n");
   for (i=0; i<100000000; i++) x++;  
   printf("Thread 1: x = %d\n", x); 
   printf("Thread 1 - done\n");
}

func2(void* arg)
{int i = 0;
   printf("      Thread 2 - running\n");
   for (i=0; i<100000000; i++) x++;
   printf("      Thread 2: x = %d\n", x); 
   printf("      Thread 2 - done\n");
}

main()
{
   stack = malloc(60000);
   printf("starting\n");
   int pid = clone(&func2, stack+10000, CLONE_VM|CLONE_FS, 0);
   printf("cloned: %d\n", pid);
   func1();
   sleep(1);
   printf("final value %d\n", x);
}


