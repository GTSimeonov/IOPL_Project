#ifndef GSTACK_C
#define GSTACK_C GSTACK_C
#include<stdlib.h>//malloc free et alii


typedef struct Stack Stack;

struct Stack
{
    struct Stack_Node
    {
        struct Stack_Node *next;
        void *data;
    }*first;
    size_t size;
};

void Stack_Init(Stack *stack)
{
    stack->size=0;
    stack->first=NULL;
}
void Stack_Push(Stack *stack,void* data)
{
    struct Stack_Node *temp_node=malloc(sizeof(struct Stack_Node));
    temp_node->data=data;
    temp_node->next=stack->first;
    stack->first=temp_node;
    ++stack->size;
}
void* Stack_Pop(Stack *stack)
{
    void* return_value=NULL;
    if(stack->first!=NULL)
    {
        struct Stack_Node *temp_first=stack->first;
        return_value=stack->first->data;

        --stack->size;
        stack->first=stack->first->next;
        free(temp_first);
    }

    return return_value;
}

#endif//#ifndef GSTACK_C

