#ifndef GQUEUE_C
#define GQUEUE_C GQUEUE_C
#include "queue.h"

void Queue_Init(Queue *q)
{
	q->first=q->last=NULL;
	q->size=0;
}
void Queue_Push(Queue *q,void *data)
{
	if(q==NULL)return;
	if(q->first==NULL)
	{
		q->first=q->last=malloc(sizeof(struct Queue_Node));	

		q->first->data=data;
		q->last->prev=NULL;

		++q->size;
		return;
	}else
	{
		struct Queue_Node *temp=malloc(sizeof(struct Queue_Node));
		q->last->prev=temp;
		temp->data=data;
		temp->prev=NULL;

		q->last=temp;
		++q->size;
	}

}
/*max queueiness*/
void Queue_Push_Front(struct Queue *q,void *data)
{
	if(q==NULL)return;
	if(q->first==NULL)
	{
		q->first=q->last=malloc(sizeof(struct Queue_Node));	

		q->first->data=data;
		q->last->prev=NULL;

		++q->size;
	}else
	{
		struct Queue_Node *temp=malloc(sizeof(struct Queue_Node));
		temp->prev=q->first;
		temp->data=data;

		q->first=temp;
		++q->size;
	}

}
void* Queue_Pop(Queue *q)
{
	if(q==NULL)return NULL;
	if(q->size==0)return NULL;

	void *return_value=q->first->data;

	if(q->size==1)
	{
		free(q->last);
		q->first=q->last=NULL;
		q->size=0;
	}else
	{
		struct Queue_Node *temp_first=q->first;
		q->first=q->first->prev;
		free(temp_first);
		--q->size;
	}
	return return_value;
}
void Queue_Append(struct Queue *lead,struct Queue *follower)
{
	assert(lead!=NULL);	
	lead->last->prev=follower->first;
	lead->last=follower->last;

	lead->size+=follower->size;
}

void Queue_Destroy(Queue *q)
{
	
	struct Queue_Node *temp_first;
	while(q->first!=NULL)
	{
		temp_first=q->first;
		q->first=q->first->prev;
		free(temp_first->data);
		free(temp_first);	
	}	

}
/*
	Fits a new Queue on top of q. It uses its elements as its own	
	Do not destroy
	last element ->prev is may not be null
 */
struct Queue Queue_Fit(struct Queue *q,size_t offset)
{
	assert(q->size>offset && offset>0);

	struct Queue ret;
	size_t i;

	ret.size=q->size-offset;
	ret.first=q->first;
	ret.last=q->last;

	for(i=0;i<offset;++i)
	{
		ret.first=ret.first->prev;
	}
	return ret;
	
}
#endif //#ifndef GQUEUE_C
