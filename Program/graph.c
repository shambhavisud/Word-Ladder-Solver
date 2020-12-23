//Word Ladder project: Graph Data Structure - CSD201
//Aditi Rai (CSE), Kritika Misra (CSE), Rimjhim (CSE), Shambhavi Sud (CSE), Sreya Chilakamarri (EEE)

//The project builds the shortest possible word ladder from one word to another based on the Dictionary by Merriam-Webster
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "avltree.h"
#include "stack.h"
#include "queue.h"
#include "graph.h"
#include "limits.h"



Graph *CreateGraph(int N)
{
  Graph *G;
  int    i;

  if (N < 1)
  {
    printf("\n**Error in CreateGraph: invalid parameter N (%d)\n\n", N);
    return NULL;
  }

  //Allocating a graph header
  G = (Graph *)malloc(sizeof(Graph));
  if (G == NULL)
  {
    printf("\n**Error in CreateGraph: malloc failed to allocate\n\n");
    exit(-1);
  }

  //alocatinf memory to array of adjacency list, one per vertex
  G->Vertices = (Edge **)malloc(N * sizeof(Edge *));
  if (G->Vertices == NULL)
  {
    printf("\n**Error in CreateGraph: malloc failed to allocate\n\n");
    exit(-1);
  }

  for (i = 0; i < N; ++i)
    G->Vertices[i] = NULL;

  //allocating memory to store the name of each vertex
  G->Names = (char **)malloc(N * sizeof(char *));
  G->NamesTree = NULL;
  if (G->Names == NULL)
  {
    printf("\n**Error in CreateGraph: malloc failed to allocate\n\n");
    exit(-1);
  }

  for (i = 0; i < N; ++i)  // initialising to names array to NULL
    G->Names[i] = NULL;


  G->NumVertices = 0;
  G->NumEdges = 0;
  G->Capacity = N;


  return G;
}



int AddVertex(Graph *G, char *name)
{
  int v = G->NumVertices;  //assigning the next free location

  if (G->NumVertices == G->Capacity)  //if the graph is full
  {
    // dynamically allocating more space hence doubling in size,we can even increase further
    int N = 2 * G->Capacity;

    // increasing size of the array of names
    char **newNames = (char **)malloc(N * sizeof(char *));
    if (newNames == NULL)
    {
      printf("\n**Error in AddVertex: malloc failed to allocate\n\n");
      exit(-1);
    }

    // copying the already existing names
    int  i;

    for (i = 0; i < G->NumVertices; ++i)
    {
      newNames[i] = G->Names[i];
    }

    free(G->Names);

    //growing the list of edges
    Edge **newVertices = (Edge **)malloc(N * sizeof(Edge *));
    if (newVertices == NULL)
    {
      printf("\n**Error in AddVertex: malloc failed to allocate\n\n");
      exit(-1);
    }

    // copying the already existing list of edges:
    for (i = 0; i < G->NumVertices; ++i)
    {
      newVertices[i] = G->Vertices[i];
    }

    free(G->Vertices);

    //finally we update the graph headers
    G->Names = newNames;
    G->Vertices = newVertices;
    G->Capacity = N;
  }

  // initializing list of edges
  G->Vertices[v] = NULL;

  //allocating memory to create copy of name

  G->Names[v] = (char *)malloc(((int)(strlen(name) + 1)) * sizeof(char));
  if (G->Names[v] == NULL)
  {
    printf("\n**Error in AddVertex: malloc failed to allocate\n\n");
    exit(-1);
  }

  strcpy(G->Names[v], name);


  G->NumVertices++;

  AVLElementType T;
  T.Vertex = v;
  strcpy(T.Word, name);
  G->NamesTree = Insert(G->NamesTree, T); //Insert func present in avltree.c

  //Returns the vertex number:
  return v;
}


int Word2Vert(Graph *G, char *Name)
{
  int  i;
  AVLElementType value;
  strcpy(value.Word, Name);
  AVLNode * T = Contains(G->NamesTree, value);
  if(T != NULL)
    return T->value.Vertex;
  return -1;

  //
  // linear search through the Names array:
  //
  for (i = 0; i < G->NumVertices; ++i)
  {
    if (strcmp(G->Names[i], Name) == 0)
      return i;
  }

  // if get here, not found:
  return -1;
}


char *Vert2Word(Graph *G, Vertex v)
{
  if (v < 0 || v >= G->NumVertices)
    return NULL;

  return G->Names[v];
}


int AddEdge(Graph *G, Vertex src, Vertex dest, int weight)
{
  if (src < 0 || src >= G->NumVertices)  // invalid vertex #:
    return 0;
  if (dest < 0 || dest >= G->NumVertices)  // invalid vertex #:
    return 0;

  //
  // allocate memory for new edge:
  //
  Edge *edge = (Edge *)malloc(sizeof(Edge));
  if (edge == NULL)
  {
    printf("\n**Error in AddEdge: malloc failed to allocate\n\n");
    exit(-1);
  }

  //
  // store data:
  //
  edge->src = src;
  edge->dest = dest;
  edge->weight = weight;

  //
  // link into edge list --- we want to insert in order so that we
  // can detect multi-edges more easily (i.e. they will be consecutive
  // in the adjacency list):
  //
  Edge *prev = NULL;
  Edge *cur = G->Vertices[src];

  while (cur != NULL)
  {
    if (dest <= cur->dest)  // insert here!
      break;

    // else keep going:
    prev = cur;
    cur = cur->next;
  }

  if (prev == NULL)  // update head pointer:
  {
    edge->next = G->Vertices[src];
    G->Vertices[src] = edge;
  }
  else  // insert between prev and cur:
  {
    edge->next = prev->next;
    prev->next = edge;
  }


  G->NumEdges++;

  return 1;
}


Vertex *Neighbors(Graph *G, Vertex v)
{
  Vertex *neighbors;
  int     N;
  int     i;

  if (v < 0 || v >= G->NumVertices)  // invalid vertex
    return NULL;

  //
  // allocate array of worst-case size: # of vertices + 1
  //
  N = G->NumVertices + 1;

  neighbors = (Vertex *)malloc(N * sizeof(Vertex));
  if (neighbors == NULL)
  {
    printf("\n**Error in Neighbors: malloc failed to allocate\n\n");
    exit(-1);
  }

  //
  // Now loop through the list of edges and copy the dest
  // vertex of each edge:
  //
  Edge *cur = G->Vertices[v];

  i = 0;
  while (cur != NULL)  // for each edge out of v:
  {
    //
    // the dest is our neighbor --- however, we have to be
    // careful of multi-edges, i.e. edges with the same dest.
    // Since edges are stored in order, edges with same dest
    // appear next to each other in the list --- so look to
    // see if array already contains dest before we copy over:
    //
    if (i == 0)  // first neighbor, we always copy:
    {
      neighbors[i] = cur->dest;
      ++i;
    }
    else if (neighbors[i - 1] != cur->dest)  // make sure not a multi-edge:
    {
      neighbors[i] = cur->dest;
      ++i;
    }
    else // multi-edge, so ignore:
      ;

    cur = cur->next;
  }

  //
  // follow last element with -1 and return:
  //
  neighbors[i] = -1;

  return neighbors;
}

int PopMin(Queue *unvisitedQ, int distance[])
{
  int    N = unvisitedQ->NumElements;
  Stack *S = CreateStack(N);
  assert(!isEmptyQueue(unvisitedQ));
  //
  // search for smallest vertex distance in the queue,
  // saving other elements in a stack so we can put back
  // later:
  //
  int minV = Dequeue(unvisitedQ);  // assume first vertex is the min:
  while (!isEmptyQueue(unvisitedQ))  // look for smaller vertex:
  {
    int v = Dequeue(unvisitedQ);
    if (distance[v] < distance[minV])
    {
      Push(S, minV);  // save so we can put back in queue:
      minV = v;  // our new min:
    }
    else  // not smaller, so save to enqueue later:
    {
      Push(S, v);
    }
  }
  //
  // empty the stack back into the queue:
  //
  while (!isEmptyStack(S))
  {
    Enqueue(unvisitedQ, Pop(S));
  }
  // at this point queue is now one smaller, with
  // min element removed.  Let's confirm:
  assert(unvisitedQ->NumElements == (N - 1));
  //
  // done:
  //
  DeleteStack(S);
  return minV;
}




Vertex *Dijkstra(Graph *G, Vertex src, Vertex dest)
{
  int  INF = INT_MAX;//infinity
  if (src < 0 || src >= G->NumVertices)  // invalid vertex
    return NULL;
  if (dest < 0 || dest >= G->NumVertices)  // invalid vertex
    return NULL;
  //
  // allocate distances array:
  //
  int N = G->NumVertices;
  int *distance = (int *)malloc(N * sizeof(int));
  if (distance == NULL)
  {
    printf("\n**Error in Dijkstra: malloc failed to allocate\n\n");
    exit(-1);
  }
  Vertex *predecessor = (Vertex *)malloc(N * sizeof(Vertex));
  if (predecessor == NULL)
  {
    printf("\n**Error in Dijkstra: malloc failed to allocate\n\n");
    exit(-1);
  }
  //
  // for each vertex, add to Queue to be visited,
  // initialize distance to Infinity, and set predecessor
  // to -1:
  //
  Queue *unvisitedQ = CreateQueue(N);
  int currV;
  for (currV = 0; currV < N; ++currV)
  {
    Enqueue(unvisitedQ, currV);
    distance[currV] = INF;
    predecessor[currV] = -1;
  }
  //
  // starting vertex has a distance of 0 from itself:
  //
  distance[src] = 0;
  //
  // Now run Dijkstra's algorithm:
  //
  while (!isEmptyQueue(unvisitedQ))
  {
    //
    // find the vertex with the smallest distance from
    // the start, that's the vertex to explore next:
    //
    currV = PopMin(unvisitedQ, distance);
    // is there a path to the current vertex?  if not,
    // there's no point in looking at neighbors...
    if (distance[currV] == INF)
      break;
    //
    // now see if we have found any shorter paths for minV's
    // neighboring vertices:
    //
    Vertex *neighbors = Neighbors(G, currV);
    int i = 0;
    while (neighbors[i] != -1)  // for each neighbor:
    {
      int adjV = neighbors[i];

      int altDistance = distance[currV];
     if (altDistance < distance[adjV])
      {
        distance[adjV] = altDistance;
        predecessor[adjV] = currV;
      }
      ++i;
    }
    free(neighbors);
  }
  //
  // Okay, algorithm has run to completion, and the path (if
  // any) is stored backwards in predecessor array.  So let's
  // use a stack to reverse it:
  //
 Stack *S = CreateStack(N);
  int v = dest;
  while (predecessor[v] != -1)
  {
    Push(S, v);
    v = predecessor[v];
  }
  // loop stops when it gets to src, so push src to finish:
  Push(S, src);
  //
  // at this point we have the path on the stack, or if there's
  // no path, just src is on the stack.  Allocated an array and
  // fill with path, or if no path, just -1:
  //
  Vertex  *path;
  if (S->NumElements == 1) // just src on stack, no path:
  {
    N = 1;  // just the -1:
    path = (Vertex *)malloc(N * sizeof(Vertex));
    if (path == NULL)
    {
      printf("\n**Error in Dijkstra: malloc failed to allocate\n\n");
      exit(-1);
    }
    path[0] = -1;  // no path from src to dest:
  }
  else
  {
    N = S->NumElements + 1;  // path + -1 at the end
    path = (Vertex *)malloc(N * sizeof(Vertex));
    if (path == NULL)
    {
      printf("\n**Error in Dijkstra: malloc failed to allocate\n\n");
      exit(-1);
    }
    // now empty the stack into the path array:
    int i = 0;
    while (!isEmptyStack(S))
    {
      path[i] = Pop(S);
      ++i;
    }
    path[i] = -1;  // need -1 terminator at the end:
  }

  DeleteStack(S);
  DeleteQueue(unvisitedQ);
  free(distance);
  free(predecessor);
  return path;
}


