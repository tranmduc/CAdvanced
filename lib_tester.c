//Testing the lib's important function

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "lib/standard/jrb_graph_directed_weighted.h"
#include "lib/standard/queue.h"
#include "lib/standard/stack.h"

Graph initGraph(){
    Graph g = createGraph();
    char str[10];

    addVertex(g, 0, "CS102"); 
    addVertex(g, 1, "CS140");
    addVertex(g, 2, "CS160"); 
    addVertex(g, 3, "CS302");
    addVertex(g, 4, "CS311"); 
    addVertex(g, 5, "MATH300");
    addDirectedEdge(g, 0, 1, 2.0); 
    addDirectedEdge(g, 0, 2, 1.0);
    addDirectedEdge(g, 0, 3, 3.0); 
    addDirectedEdge(g, 1, 3, 4.5); 
    addDirectedEdge(g, 5, 4, 6.5); 
    addDirectedEdge(g, 3, 4, 3.0);
    addDirectedEdge(g, 2, 3, 1.0);
    //addUnitDirectedEdge(g, 3, 0);

    return g;
}

int testGraph(){
    int i, n, output[100];
    int output2[100];

    Graph g = initGraph();

    addVertex(g, 0, "Object");

    printf("Edge 0 to 1 length: %.2f\n", getEdgeValue(g, 0, 1));
    removeEdge(g, 0, 1);
    removeEdge(g, 0, 1);

    printf("Vertice %d has name %s\n", 0, getVertex(g, 0));
   
   //test adjancency
   printf("hasEdge from 0 to 2: %d\n", hasEdge(g, 0, 2));
   
   //print
   printf("Full graph:\n");
   printGraph(g);

   //get in vertices
   int vertex = 2;
   n = inDegree(g, vertex, output);
   if (n==0) printf("No in_vertices to node %d\n", vertex);
   else {   
      printf("In vertices of node %d:", vertex);
      for (i=0; i<n; i++) printf("%d ", output[i]);
   }
   printf("\n");


   //get out vertices
   n = outDegree(g, vertex, output2);
   if (n==0) printf("No out_vertices to node %d\n", vertex);
   else {   
      printf("Out vertices of node %d:", vertex);
      for (i=0; i<n; i++) printf("%d ", output2[i]);
   }
   printf("\n");

   
   
   //drop
   dropGraph(g);
    return 0;
}

int testBDFS(){
    int i, n, output[100];
    Graph g = initGraph();

    //print
    printf("Full graph:\n");
    printGraph(g);
    
    void print_int(int n){
        printf("Visited %d\n", n);
    }
    //Find way from  start to stop
    int start = 0;
    int stop = 5;
    printf("Finding a way from node %d to %d using BFS\n", start, stop);
    BFS(g, start, stop, print_int);

    printf("Finding a way from node %d to %d using DFS\n", start, stop);
    DFS(g, start, stop, print_int);

    //drop
    dropGraph(g);
    return 0;
}

int testDAG(){
    Graph g = initGraph();

    printf("Is graph DAG: %d\n", isDAG(g));

    dropGraph(g);
    return 0;
}

int testTSort(){
    Graph g = initGraph();
    int output[GRAPH_MAX_SIZE], n;
    topologicalSort(g, output, &n);

    printf("Topological order of graph: ");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", output[i]);
        //printf("%s ", getVertex(g, output[i]));
    }
    return 0;
    
}

int testDijkstra(){
    Graph g = initGraph();
    int path[GRAPH_MAX_SIZE];
    int num_ver_on_path;

    int start = 3;
    int stop = 2;

    double path_length = findShortestPath(g, start, stop, path, &num_ver_on_path);
    int doesPathExist = !(path_length == INFINITY_LENGTH);

    printf("Path exists from %d to %d: %d\n", start, stop, doesPathExist);
    if(doesPathExist){
        printf("Path length: %.2f\nNum vertices on path: %d\n", path_length, num_ver_on_path);
    }
}
int main(){
    //testGraph();
    //testDAG();
    //testBDFS();
    //testTSort();
    testDijkstra();
}