#include <stdio.h>
#include <unistd.h>
#include "threads.h"

/*
Authors: 
Murali Ravivarma
Vivek Narasimha Dondeti
*/

struct TCB_t *ReadyQ;
int global = 1;

// function prototypes
void function1();
void function2();
void function3();

// function pointers
void (*f1)();
void (*f2)();
void (*f3)();

// gcc -o thread_test.o thread_test.c
int main(void) {
	ReadyQ = newQueue();
	f1 = function1; start_thread(f1);
	f2 = function2; start_thread(f2);
	f3 = function3; start_thread(f3);
	run();
  return 0;
}

void function1() {
	int local = 1;
	while(1) {
    		printf("\nf1 local = %d; ", local);
		local++;
		printf("global = %d\n", global);
		global++;
		sleep(1);
		yield(); // context switch
	}
}

void function2() {
	int local = 1;
	while(1) {
		printf("f2 local = %d; ", local);
		local++;
		printf("global = %d\n", global);
		global++;
		sleep(1);
		yield(); // context switch
	}
}

void function3() {
	int local = 1;
	while(1) {
		printf("f3 local = %d; ", local);
		local++;
		printf("global = %d\n", global);
		global++;
    		sleep(1);
    		yield(); // context switch
	}
}
