//Word Ladder project: Graph Data Structure - CSD201
//Aditi Rai (CSE), Kritika Misra (CSE), Rimjhim (CSE), Shambhavi Sud (CSE), Sreya Chilakamarri (EEE)

//The project builds the shortest possible word ladder from one word to another based on the Dictionary by Merriam-Webster
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "stack.h"



Stack *CreateStack(int N)
{
  Stack *S;

  if (N < 1)
  {
    printf("\n**Error in CreateStack invalid parameter N (%d)\n\n", N);
    return NULL;
  }


  S = (Stack *)malloc(sizeof(Stack));
  if (S == NULL)
  {
    printf("\n**Error in CreateStack: malloc failed to allocate\n\n");
    exit(-1);
  }



  S->Elements = (StackElmt *)malloc(N * sizeof(StackElmt));
  if (S->Elements == NULL)
  {
    printf("\n**Error in CreateStack: malloc failed to allocate\n\n");
    exit(-1);
  }


  S->Top = -1;  // first push will be ++Top
  S->NumElements = 0;
  S->Capacity = N;


  return S;
}


void DeleteStack(Stack *S)
{
  free(S->Elements);
  free(S);
}

//
// isEmptyStack:
//
// Returns true (non-zero) if stack is empty, false (0) if not.
//
int isEmptyStack(Stack *S)
{
  return S->NumElements == 0;
}




int Push(Stack *S, StackElmt e)//StackElementType e)
{
  if (S->NumElements == S->Capacity)  // full
  {
    //// The stack will dynamically grow in size if it becomes
// full, doubling its internal capacity each time.
    // instead of failing, let's reallocate the
    // underlying array to be twice as big:
    //
    int N = 2 * S->Capacity;

    StackElmt *newE = (StackElmt *)malloc(N * sizeof(StackElmt));

    if (newE == NULL)
    {
      printf("\n**Error in Push: malloc failed to allocate\n\n");
      exit(-1);
    }

    // copy elements over:
    int  i;

    for (i = 0; i <= S->Top; ++i)
    {
      newE[i] = S->Elements[i];
    }

    free(S->Elements);

    S->Elements = newE;
    S->Capacity = N;
  }

  //
  // there's room, add to *END* since that's faster than
  // shifting everyone to the right:
  //
  S->Top++;

  S->Elements[S->Top] = e;
  S->NumElements++;

  return 1;//true
}


StackElmt Pop(Stack *S)
{
  if (isEmptyStack(S))  // nothing to pop!
  {
    printf("\n**Error in Pop: S is empty?!\n\n");
    exit(-1);
  }

 StackElmt e = S->Elements[S->Top];

  S->Top--;
  S->NumElements--;

  return e;
}
