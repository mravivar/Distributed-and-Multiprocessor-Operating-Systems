#include "msgs.h"
#include <stdlib.h>
#include <time.h>

// Murali Ravivarma
// Vivek Narasimha Dondeti

void client()
{
    srand(time(NULL));
    int r;
    int count1=1;
    while(1)
    {
        int a[10];
        int i=0;  
        r=(rand()%100);
        a[0]=r;
        printf("\n\nAt Client, sending msg to Server specifying the replyport as %d\n", r);
        for(i=1;i<10;i++) 
        { 
 	          a[i]=count1;
 	          printf("%d ", a[i]); 
        }
        printf("\n");
        send(&port[0], a);
  	    count1++;
        sleep(2);
        receive(&port[r],a);
        printf("\nAt Client, receiving msg from Server on port: %d\n",r);
        for(i=1;i<10;i++) 
        { 
 	          printf("%d ", a[i]); 
        }
        sleep(2);
    }
}

void server()
{
    int count=0;
    int replyport;
    while(1){
        count++;
        int a[10];
        receive(&port[0],a);
        printf("\nAt Server, receive success \n");
        int i=0; 
   	    for(i=1;i<10;i++)
            printf("%d ", a[i]);
   	    replyport=a[0];
        printf("\nAt Server, sending the msg to Client via reply port: %d\n",replyport);
        for(i=1;i<10;i++)
            printf("%d ", a[i]);
        send(&port[replyport],a);
    }
}

void (*c1)();
void (*s1)();

int main()
{   
    ReadyQ = newQueue();
    initialiseports();

    printf("\n********************************************\n");
    printf("Message passing system with two clients and a server\n");
    printf("Server listens at port 0 and send back the msgs to the port number specified by client in msg_arr[0]\n");
	//printf("NOTE: To create second server, pls uncomment line no. 77 \n");
    printf("********************************************\n");
    c1 = client; 
    printf("\nCreating Client 1\n"); start_thread(c1);
    printf("Creating Client 2\n"); start_thread(c1);
	s1 = server;    
	printf("Creating Server 1\n"); start_thread(s1);
	//printf("Creating Server 2\n"); start_thread(s1);
    
    run();
    
    while (1)
    { 
        sleep(2);
    }
}
