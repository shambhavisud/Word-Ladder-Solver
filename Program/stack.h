#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

typedef int StackElmt;//stack element type
typedef struct Stack
{

  StackElmt *Elements;
  int  Top;
  int  NumElements;  // no. of elements currently in stack
  int  Capacity;     // max no. of elements that can fit in stack
} Stack;

Stack *CreateStack(int N);
void   DeleteStack(Stack *S);
int    isEmptyStack(Stack *S);
int    Push(Stack *S, StackElmt e);
StackElmt Pop(Stack *S);

#endif // STACK_H_INCLUDED
