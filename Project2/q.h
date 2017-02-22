#include "TCB.h"
#include <stdlib.h>
#include <stdio.h>

struct TCB_t* newQueue(){
    struct TCB_t *header = (struct TCB_t*)malloc(sizeof(struct TCB_t*));
    header->next = header;
    header->prev = header;
    return header;
}

struct TCB_t* NewItem(){
    struct TCB_t *newElement = (struct TCB_t*)malloc(sizeof(struct TCB_t*));
    return newElement;
}

void AddQueue(struct TCB_t *head, struct TCB_t *item){
    if(head == NULL){
        return;
    }
    struct TCB_t *temp = head;
    while(temp->next != head){
	     temp = temp->next;
    }
    temp->next = item;
    item->prev = temp;
    item->next = head;
    head->prev = item;
}

struct TCB_t* DelQueue(struct TCB_t *head){
    if(head->next == head){ // (or) head->prev == head
	     return NULL;
    }

    struct TCB_t *temp1 = head->next;
    struct TCB_t *temp2 = temp1->next;
    head->next=temp2;
    temp2->prev=head;
    temp1->next = NULL;
    temp1->prev = NULL;
    return temp1;
}

void display(struct TCB_t *head) {
    struct TCB_t *temp = head;
    while(temp->next!=head){
      temp=temp->next;
      printf("%p(%d) -> ", temp, temp->thread_id);
    }
    printf("\n");
}

/*
int main()
{
    struct TCB_t *head;
    struct TCB_t *item;
    int choice, counter=0;

    head = newQueue();
    printf("New Queue created!\n");
    printf("[%p(n: %p)(p: %p)(id: %d)]\n", head, head->next, head->prev, head->thread_id);
    printf("1 - Add element\n");
    printf("2 - Delete element\n");
    printf("3 - Display elements\n");
    while (1) {
       	printf("Enter choice : ");
	scanf("%d", &choice);
	switch(choice) {
	    case 1:
		item = NewItem();
  		item->thread_id=++counter;
		AddQueue(head, item);
      		printf("One element added\n");
		break;
	    case 2:
      		item=DelQueue(head);
      		if(item != NULL) {
        	    printf("Deleted element: %d\n", item->thread_id);
      		} else {
        	    printf("No element to delete\n");
      		}
		break;
	    case 3:
		display(head);
		break;
	    default:
		exit(0);
	}
    }
    return 0;
}
*/
