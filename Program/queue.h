#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED






typedef int QElement;
typedef struct Queue
{
  QElement  *Elements;  // array of elements in queue:

  int  Front;
  int  Back;
  int  NumElements;  // no. of elements currently in Q
  int  Capacity;     // max no. of elements that can fit in Q
} Queue;

Queue *CreateQueue(int N);
void   DeleteQueue(Queue *Q);
int    isEmptyQueue(Queue *Q);
int    Enqueue(Queue *Q, QElement e);
QElement Dequeue(Queue *Q);





#endif // QUEUE_H_INCLUDED
