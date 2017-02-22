#include "msgs.h"
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// Murali Ravivarma
// Vivek Narasimha Dondeti

char* fileread(char *readfile,int position,int length, int *flag)
{
 FILE *ifp;
 ifp = fopen(readfile, "r"); 
 char *buff = malloc(sizeof(char)*length);
 if(ifp!=NULL)
 {
  fseek(ifp,position,SEEK_SET);
  //printf("%d\n",position );
  fread(buff,length, 1, ifp);
 }
 //printf("%s\n",buff);
  int ch = getc(ifp);
 if (ch == EOF)
 	*flag =0; 
 fclose(ifp);
 return buff;
}

void filewrite(char *towrite, char *filename)
{
 FILE *ofp = fopen(filename, "a");
//printf("\nFile[%s], content[%s]\n", filename, towrite);
 if(ofp!=NULL)
 {
 fseek(ofp,0,SEEK_END);	
 fprintf(ofp,"%s",towrite);
}
 fclose(ofp);
 return;
}

char* fileNameG[5];
int cid=0;
void client()
{
    int replyPort = ++cid;
    char *fileName = fileNameG[replyPort-1];
	//printf("client file name:%s", fileName);
    int lengthOfFileName = strlen(fileName);
    int flag[1]={1};
    int indexOfFileContent = 0;
    
    int sending[10];   
    int rec[10];
    sending[0] = replyPort;
    sending[1] = 1;
    
    //for handshake
    while(1){
	printf("At Client %d, send flag %d", replyPort, sending[1]);
        send(&port[0], sending);
        sleep(1);
        receive(&port[replyPort], rec);
        if(rec[0] == 1){ // if 1, handshake failed
          sleep(1);
			yield();
          printf("Client %d not allowed\n", replyPort); // put this line before sleep
          continue;
        } else {
	  printf("At Client, receive flag 1 ACK success");
          break;
        }
    }
    
    // for sending filename
    sending[1] = 2;
    int i;
    int numberOfBlocks = lengthOfFileName/7; //Only 7 char per message
    int numberOfExtras = lengthOfFileName%7;
    int fileNameSendFailed = 0, contentsendFailed=0;
    for(i = 0; i < numberOfBlocks; i++){
        sending[2] = 7;
        int j;
        for(j = 0; j < 7; j++){
            int position = i * 7 + j;
            int positionInMessage = j+3;
            sending[positionInMessage] = fileName[position];
        }
        send(&port[0], sending);
        sleep(1);
        receive(&port[replyPort], rec);
        if(rec[1] == 1){
            printf("file name too large");
            fileNameSendFailed = 1;
            break;
        }
    }
    if(fileNameSendFailed == 1){
      while(1){ 
        sleep(1);
      }
    }

    if(numberOfExtras > 0){
        sending[2] = numberOfExtras;
        for(i = 0; i < numberOfExtras; i++){
            int position = i + numberOfBlocks * 7;
            int positionInMessage = 3 + i;
            sending[positionInMessage] = fileName[position];
        }
        send(&port[0], sending);
        receive(&port[replyPort], rec);
        if(rec[1] == 1){
            printf("file name too large\n");
            fileNameSendFailed = 1;
        }
    }
    if(fileNameSendFailed == 1){
      while(1){ 
        sleep(1);
		yield();
      }
    }
    //printf("Client Completed sending filename %d\n ", sending[1]);
    printf("Client %d Completed sending filename\n", replyPort);
    
    // for sending filecontent
    printf("At Client %d, send flag %d", replyPort, sending[1]);
    sending[1] = 3;

	while(*flag){
		char* content=fileread(fileName, indexOfFileContent, 7, flag);
		//printf("\nContent[%s]\n", content);
		sending[2] = strlen(content);
        int j;
        for(j = 0; j < strlen(content); j++){
            //int position = i * 7 + j;
            //int positionInMessage = j+3;
            sending[j+3] = content[j];			
        }
		indexOfFileContent+=strlen(content);
        send(&port[0], sending);
		//send message
        receive(&port[replyPort], rec);
        if(rec[1] == 1){
            printf("file content too large");
            contentsendFailed = 1;
            break;
        }
    }
    if(contentsendFailed == 1){
      while(1){ 
        sleep(1);
		yield();
      }
    }
    send(&port[0], sending);
    receive(&port[replyPort], rec);
    printf("\nAt client %d, part 3 receive ack success\n", replyPort);
    while(1){
      sleep(1);
    }
}

void server()
{
    char *fileName[6];
    int k=0;
    for(k=0;k<6;k++){
        fileName[k] = (char *)malloc((15+7+1)*sizeof(char));
        strcpy(fileName[k],"");
    }
    
    int fileNameSize[6] = {0,0,0,0,0, 0};
    long fileContentSize[5] = {0,0,0,0,0};
	bool renamedServer[5]={0, 0, 0, 0, 0};
    
    int concurrentTransfer = 0;
    
    //int ctr=1;
    //while(ctr){
      //printf("\nctr = %d\n",ctr);
      //ctr++;
    while(1) {
        int a[10];
        int reply[10];
        int i = 0;
        for(i=0;i<10;i++){
          reply[i] = 0;
        }
        receive(&port[0],a);
        
        int replyport = a[0];
        int flag = a[1];
	printf("\nAt Server, receive flag %d success \n", flag);
        //printf("flag = %d \n", flag);
        if(flag == 0){
            continue;
        }
        // Flag 1 is asking to connect to the server.
        if(flag == 1){
            if(concurrentTransfer > 3){ //check here
              reply[0] = 1;
            } else {
              reply[0] = 0;
            }
            concurrentTransfer = concurrentTransfer + 1;
	    printf("\nAt Server, send flag %d ACK \n", flag);
            send(&port[replyport], reply);

        } else if(flag == 2){ //Flag 2 is for sending file name.
            int messageSize = a[2];
            fileNameSize[replyport] = fileNameSize[replyport] + messageSize;
            if(fileNameSize[replyport] > 15){
                reply[1] = 1;
                send(&port[replyport], reply);
                fileNameSize[replyport] = 0;
				continue;
            }
			char tmp[8];
            for(i=3;i<=messageSize + 2;i++){
				tmp[i-3] = (char)a[i];                
            }			
			tmp[7]='\0';
			strcat(fileName[replyport], tmp);
            printf("At Server, Filename sent by client %d is %s \n", replyport, fileName[replyport]);
            reply[1] = 0;
            send(&port[replyport], reply);
        } else if(flag == 3){
			//printf("file Name");
			char towrite[8];
			int i;
			if(!renamedServer[a[0]-1]){
				//printf("filename[%s]", fileName[replyport]);
				strcat(fileName[replyport], ".server");
				//printf("filename[%s]", fileName[replyport]);
				renamedServer[a[0]-1]=1;
			}
            //printf("Filename sent by port %d is %s \n", replyport, fileName[replyport]);
	    printf("\nDo file content operation\n");
		fileContentSize[a[0]]+=a[2];
		//todo replace the value
		if(fileContentSize[a[0]]>100000){
			reply[0] = 1;
			send(&port[replyport], reply);              
			continue;
		}
		reply[0] = 0;
	    //covert array in char *
		for(i=0;i<a[2];i++){
			towrite[i]=a[i+3];
		}
		towrite[a[2]]='\0';
		filewrite(towrite, fileName[a[0]]);
		send(&port[replyport], reply);    
	    /*
	    int messageSize = a[2];
            fileContentSize[replyport] = fileContentSize[replyport] + messageSize;
            if(fileContentSize[replyport] > 15){
                reply[1] = 1;
                send(&port[replyport], reply);
                fileContentSize[replyport] = 0;
            }
            for(i=3;i<=messageSize + 2;i++){
                char popo = a[i];
                char *p = &popo;
                strcat(fileName[replyport], p);
            }
            
            printf("At Server, Filename sent by client %d is %s \n", replyport, fileName[replyport]);
            reply[1] = 0;
            send(&port[replyport], reply);
	    */
	    
	    
	    
        }
    }
}

void (*c1)();
void (*s1)();
int main(int argc, char *arg[])
{
    ReadyQ = newQueue();
    initialiseports();
    printf("\n********************************************\n");
    printf("Client Server Program\n");
    printf("********************************************\n");

    fileNameG[0]=arg[2];
    fileNameG[1]=arg[3];
    fileNameG[2]=arg[4];
	fileNameG[3]=arg[5];
	fileNameG[4]=arg[6];

    c1 = client;
    printf("\nCreating Client 1\n");
    start_thread(c1);
start_thread(c1);
start_thread(c1);
start_thread(c1);
start_thread(c1);
    //printf("Creating Client 2\n"); start_thread(c1);
    s1 = server;
    printf("\nCreating Server\n");
    start_thread(s1);
    
    //printf("Creating Server 2\n"); start_thread(s1);
    run();
    while (1)
    {
        sleep(2);
    }
}
