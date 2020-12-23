//Word Ladder project: Graph Data Structure - CSD201
//Aditi Rai (CSE), Kritika Misra (CSE), Rimjhim (CSE), Shambhavi Sud (CSE), Sreya Chilakamarri (EEE)

//The project builds the shortest possible word ladder from one word to another based on the Dictionary by Merriam-Webster


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "avltree.h"
#include "graph.h"





Graph *Read_and_AddWords(char *fname)
{
  FILE  *f;
  char   line[256];
  int    linesize = sizeof(line) / sizeof(line[0]);

  f = fopen(fname, "r");//r means read mode

  if (f == NULL)
  {
    printf("**ERROR: '%s' not found\n\n", fname);
    exit(-1);
  }


  printf(">>>>Building Graph from '%s'...\n", fname);

  Graph *G = CreateGraph(256);  // keeping 256 as the initial capacity of the graph:

  fgets(line, linesize, f);

  while (!feof(f))
  {
    line[strcspn(line, "\r\n")] = '\0';  // strip EOL(s) char at end:

    if (AddVertex(G, line) < 0)
    {
      printf("**Error: AddVertex failed?!\n\n");
      exit(-1);
    }

    fgets(line, linesize, f);
  }


  fclose(f);

  return G;
}



void AddEdges(Graph *G)
{

  int  v;

  for (v = 0; v < G->NumVertices; ++v)
  {
    char *word = G->Names[v];

    char *temp = (char *)malloc(((int)(strlen(word) + 1)) * sizeof(char));

    int  i;
    for (i = 0; i < (int)strlen(word); ++i)
    {
      strcpy(temp, word);

      char  c = 'a';
      while (c <= 'z')
      {
        temp[i] = c;  // change one letter:

        int v2 = Word2Vert(G, temp);
        if (v2 >= 0 && v2 != v)  // dest exists, add edge:
        {
          if (!AddEdge(G, v, v2, 1))
          {
            printf("**Error: AddEdge failed?!\n\n");
            exit(-1);
          }
        }

        ++c;
      }
    }

    free(temp);
  }
}


int main()
{
  Graph *G;
  char  *fname = "dictionary.txt";
  char   line[256];
  int    linesize = sizeof(line) / sizeof(line[0]);

  printf("**HI! Welcome to the Word Ladder Program **\n\n");

  //
  // (1) input words and insert each word as a vertex:
  //


  G = Read_and_AddWords(fname);

  AddEdges(G);




  printf("\n");


  printf("Please Enter a Word from the Dictionary (**press ENTER to quit**):  ");
  int count = 1;
  fgets(line, linesize, stdin);
  line[strcspn(line, "\r\n")] = '\0';  // strip EOL(s) char at end:

  int v1, v2;
  Vertex *ladder;
  while (strlen(line) > 0)
  {
    v1 = Word2Vert(G, line);

    if (v1 < 0)
    {
      printf("Word %d not found, please try again...\n", count++);
    }
    else
    {
      printf("Please Enter another Word from the Dictionary (**press ENTER to quit**): ");
      fgets(line, linesize, stdin);
      line[strcspn(line, "\r\n")] = '\0';  // strip EOL(s) char at end:
      if (strlen(line) > 0)
      {
        v2 = Word2Vert(G, line);
        if (v2 < 0)
        printf("Word %d not found, please try again...\n", count++);
        else
        {

          ladder = Dijkstra(G,v1,v2);
          if(ladder[0] == -1)
            printf("** There is no word ladder from '%s' to '%s'. \n", Vert2Word(G,v1), Vert2Word(G,v2));
          else
          {
            int i;
            printf("** Shortest word ladder ** \n   ");
            for(i = 0; ladder[i] != -1; i++)
              printf("%s\n   ", Vert2Word(G,ladder[i]));
            printf("Length: %d\n", i-1);
                      }
        }
      }
    }

    printf("\n");
    printf("Please Enter a Word from the Dictionary (**press ENTER to quit**): ");

    fgets(line, linesize, stdin);
    line[strcspn(line, "\r\n")] = '\0';  // strip EOL(s) char at end:
  }



  printf("\nExit successful!\n");




  return 0;
}
