#include "sem.h"

// Each port has a mutex semaphore, a producer semaphore and a consumer semaphore
typedef struct port
{
	int msg[10][10];
	int in, out;
	struct semaphore_t *mutex, *full, *empty;
}PORT;
	
PORT port[100];
	
void initialiseports()
{
	for(int i=0;i<100;i++)
	{
		port[i].in=0;
		port[i].out=0;
		port[i].mutex=CreateSem(1);
		port[i].full=CreateSem(10);
		port[i].empty=CreateSem(0);
	}
}
	

void send(PORT *port, int *msg)
{
	P(port->full);
	P(port->mutex);
	int i=0;
	for(i=0;i<10;i++)
	{
		port->msg[port->in][i] = msg[i];
	}
	port->in = ((port->in)+1)%10;
	V(port->mutex);
	V(port->empty);
}

	
void receive(PORT *port, int *msg)
{
	P(port->empty);
	P(port->mutex);
	int i=0;
	for(i=0;i<10;i++)
	{
		msg[i]=port->msg[port->out][i];
	}
	port->out = ((port->out)+1)%10;
	V(port->mutex);
	V(port->full);
}


