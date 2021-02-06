#ifndef GQUEUE_H
#define GQUEUE_H GQUEUE_H
#include<stdlib.h>//malloc free
#include<assert.h>

typedef struct Queue Queue;

struct Queue
{
	struct Queue_Node
	{
		void *data;
		struct Queue_Node *prev;
	}*first,*last;
	
	size_t size;
	
};
void Queue_Init(Queue *q);
void Queue_Push(Queue *q,void *data);
void Queue_Push_Front(struct Queue *q,void *data);
void* Queue_Pop(Queue *q);
void Queue_Append(struct Queue *lead,struct Queue *follower);
void Queue_Destroy(Queue *q);

#endif
