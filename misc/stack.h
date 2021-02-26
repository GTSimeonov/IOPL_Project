#ifndef GSTACK_H
#define GSTACK_H GSTACK_H
#include<stdlib.h>
typedef struct Stack Stack;

struct Stack_Node
{
	struct Stack_Node *next;
	void *data;
};
struct Stack
{
	struct Stack_Node *first;
	size_t size;
};

void Stack_Init(Stack *stack);
void Stack_Push(Stack *stack,void* data);
void* Stack_Pop(Stack *stack);


#endif
