//Word Ladder project: Graph Data Structure - CSD201
//Aditi Rai (CSE), Kritika Misra (CSE), Rimjhim (CSE), Shambhavi Sud (CSE), Sreya Chilakamarri (EEE)

//The project builds the shortest possible word ladder from one word to another based on the Dictionary by Merriam-Webster

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "queue.h"




Queue *CreateQueue(int N)
{
  Queue *Q;

  if (N < 1)
  {
    printf("\n**Error in CreateQueue invalid parameter N (%d)\n\n", N);
    return NULL;
  }


  Q = (Queue *)malloc(sizeof(Queue));
  if (Q == NULL)
  {
    printf("\n**Error in CreateQueue: malloc failed to allocate\n\n");
    exit(-1);
  }


  Q->Elements = (QElement *)malloc(N * sizeof(QElement));
  if (Q->Elements == NULL)
  {
    printf("\n**Error in CreateQueue: malloc failed to allocate\n\n");
    exit(-1);
  }


  Q->Front = 0;  // first dequeue from position 0
  Q->Back = -1;  // first enqueue will be ++Back
  Q->NumElements = 0;
  Q->Capacity = N;


  return Q;
}


void DeleteQueue(Queue *Q)
{
  free(Q->Elements);
  free(Q);
}


// Returns true (non-zero) if queue is empty, false (0) if not.

int isEmptyQueue(Queue *Q)
{
  return Q->NumElements == 0;
}



// The queue will dynamically grow in size if it becomes
// full, doubling its internal capacity each time.
int Enqueue(Queue *Q, QElement e)
{
  if (Q->NumElements == Q->Capacity)  // full
  {
    //
    // instead of failing, let's reallocate the
    // underlying array to be twice as big:
    //
    int N = 2 * Q->Capacity;

    QElement *newE = (QElement *)malloc(N * sizeof(QElement));
    if (newE == NULL)
    {
      printf("\n**Error in Enqueue: malloc failed to allocate\n\n");
      exit(-1);
    }

    // copy elements over --- for simplicity, the elements
    // will be copied over to the front of the new array:
    int  i = Q->Front;
    int  j = 0;
    int  n;

    for (n = 0; n < Q->NumElements; ++n)
    {
      newE[j] = Q->Elements[i];
      ++j;

      ++i;
      if (i >= Q->Capacity)
        i = 0;
    }

    free(Q->Elements);

    Q->Front = 0;
    Q->Back = j - 1;
    Q->Elements = newE;
    Q->Capacity = N;
  }

  //
  // there's room, add to end:
  //
  Q->Back++;

  if (Q->Back >= Q->Capacity)  // did we wrap around?
    Q->Back = 0;

  Q->Elements[Q->Back] = e;  // enqueue:
  Q->NumElements++;

  return 1;  /*true*/
}


QElement Dequeue(Queue *Q)
{
  if (isEmptyQueue(Q))
  {
    printf("\n**Error in Dequeue: Q is empty?!\n\n");
    exit(-1);
  }


  QElement e = Q->Elements[Q->Front];

  Q->Front++;
  if (Q->Front >= Q->Capacity)  // wrap around
    Q->Front = 0;

  Q->NumElements--;

  return e;
}

