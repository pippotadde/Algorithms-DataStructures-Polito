#ifndef GRAPH_H
#define GRAPH_H

#include "ST.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct edge { int v; int w; int wt; } Edge;
typedef struct graph *Graph;

Graph GRAPHinit(int V);
void  GRAPHfree(Graph G);
Graph GRAPHload(FILE *fin);
void  GRAPHinsertE(Graph G, int id1, int id2, int wt);
void  GRAPHstore(Graph G, FILE *fout);

static int dfsCycle(Graph G, int u, int *color, int *removed);
int isDAG(Graph G, int *removed);
int* costruisciDAG(Graph G, int *bestK, int *bestWeight);

void tsDFS(Graph G, int u, int *visited, int *removed, int *ts, int *tsIndex);
void  GRAPHlongestPath(Graph G, int *removed);

static void printEdgeSet(Graph G, int *removed);

#endif