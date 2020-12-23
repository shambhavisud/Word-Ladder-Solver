#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED




typedef int Vertex;

typedef struct Edge
{
  Vertex  src;
  Vertex  dest;
  int     weight;
  struct Edge *next;
} Edge;

typedef struct Graph
{
  Edge    **Vertices;
  AVLNode *NamesTree;
  char    **Names;
  int       NumVertices;
  int       NumEdges;
  int       Capacity;
} Graph;

Graph  *CreateGraph(int N);

int     AddVertex(Graph *G, char *name);
int     Name2Vertex(Graph *G, char *Name);
char   *Vertex2Name(Graph *G, Vertex v);
int     AddEdge(Graph *G, Vertex src, Vertex dest, int weight);

Vertex *Neighbors(Graph *G, Vertex v);

Vertex *Dijkstra(Graph *G, Vertex src, Vertex dest);


#endif // GRAPH_H_INCLUDED
