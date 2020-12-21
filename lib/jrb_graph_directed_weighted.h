#ifndef _JRB_GRAPH_DIRECTED_WEIGHTED_
#define _JRB_GRAPH_DIRECTED_WEIGHTED_

#include "jrb.h"

#define GRAPH_MAX_SIZE 100
#define INFINITY_LENGTH 999999

typedef struct{
    JRB edges;
    JRB vertices;
} Graph;

/** Directed Graph is represented by two JRBs; one for edges and one for vertices
 * JRB of edge:{
 *  0: {2: 1, 3: 1, 4: 2} *Note: 2:1 means node 0 connects to 2 with weight 1
 *  1: {}
 *  2: {3:1,}
 * ...
 * }
 * JRB of vertices:
 * 0 - name_1
 * 1 - name_2
 * ...
*/

Graph createGraph();

/** Add a vertex with a name and id to the graph */
void addVertex(Graph graph, int id, char* name);

/** Get name of a vertex from the graph, given the id */
char* getVertex(Graph graph, int id);

/** Check if the graph has vertex with id */
int hasVertex(Graph graph, int id);

/** Add a directed edge from v1 to v2. Return 1 if success and 0 if not*/
int addDirectedEdge(Graph graph, int v1, int v2, double weight);

/** Add two directed edge from v1 to v2 and vice versa. Return 1 if success and 0 if not*/
int addBidirectedEdge(Graph graph, int v1, int v2, double weight);

/** Add a unit directed edge (weight = 1) from v1 to v2. Return 1 if success and 0 if not*/
int addUnitDirectedEdge(Graph graph, int v1, int v2);

/**  Get weight between two vertices. return INFINITIVE_VALUE if no edge between v1 and v2*/
double getEdgeValue(Graph graph, int v1, int v2);

/** Remove edge from v1 to v2 if any*/
void removeEdge(Graph graph, int v1, int v2);

/** Remove a vertex along with its edges*/
void removeVertex(Graph graph, int v);

/** Check whether there's an edge from v1 to v2. Former: isAdjacent*/
int hasEdge(Graph graph, int v1, int v2);

/** Count all edges that point to this vertex. Store list of vertices into output[]*/
int inDegree(Graph graph, int v, int* output);

/** Count all edges that point out from this vertex. Store list of vertices into output[]*/
int outDegree(Graph graph, int v, int* output);

void dropGraph(Graph graph);

/** Given the graph, print the vertices and edges */
void printGraph(Graph graph); 

/** Print vertices of the graph
 * Vertices:
 * id 1: <name_1>
 * id 2: <name_2>
 * ...
*/
void printVerticesGraph(Graph graph);

/** Print edges of the graph
 * root
 * |- <name_1>: <name_2>
 * |- <name_2>: <name_3>
*/
void printEdgesGraph(Graph graph);

/** Check if the graph is Directed Acyclic Graph*/
int isDAG(Graph graph);

/** Traverse the graph using BFS, and act some f function on the graph's node 
 * Characteristic: directed, unweighted, feasible
 * start is the first vertex to visit
 * stop is the vertex to be visited at the end, if stop = -1, all the vertices may be visited
 * func is a pointer to the function that process on the visited vertices
 * return: number of node traveled, -1 if not found
 * */
int BFS(Graph graph, int start, int stop, void (*func)(int));

/** Traverse the graph using DFS, and act some f function on the graph's node 
 * Characteristic: directed, unweighted, feasible
 * start is the first vertex to visit
 * stop is the vertex to be visited at the end, if stop = -1, all the vertices may be visited
 * func is a pointer to the function that process on the visited vertices
 * return: number of node traveled, -1 if not found, -2 if cycle is detected
 * */
int DFS(Graph graph, int start, int stop, void (*func)(int));

/** Input: an directed acyclic graph (DAG)
 * Output: a linear order that respects the prec constraints
 * @output_array: store the output ids
 * @output_length: length of @output_array
 * Algo:
 * Build an “indegree table” of the DAG
*  Output a vertex v with zero indegree
*  For vertex v, the arc (v, w) is no longer useful since the task (vertex) w does not need to wait for v to finish anymore
 * */
void topologicalSort(Graph graph, int* output_array, int* output_length);

/** Find the shortest path from start to stop.
 * Return the total weight of the path and the path is given via path and its length.
 * path*: int array pointer of the path (v1->v3->v2)
 * num_ver_on_path*: int pointer of the path's length. Return -1 if not found
 * Return length of path. INFINITIVE_VALUE if no path is found
 * --------------------------
 * Assumption: non-negative weight edges
 * Algorithm: using Priority queue
 * For each vertex v in the graph, we maintain v.d(), the estimate of the shortest path from s, initialized to Infinity the start
 * Relaxing an edge (u,v) means testing whether we can improve the shortest path to v found so far by going through u
 * Priority queue is reorganized whenever some v.d() decreases
 *  */
double findShortestPath(Graph graph, int start, int stop, int* path, int* num_ver_on_path);

/** Relaxing an edge (intermediate_v,current_des) means testing whether we can improve the shortest path to current_des found so far by going through intermediate_v 
 * d: array pointer of estimation shortest path from start
 * parent: use to backtrace the path
 * Return: True if relaxed successfully, false otherwise
*/
int relax(Graph graph, int intermediate_v, int current_des, double* d, int* path);

/** Init the d,parent array and the priority queue
 * Priority queue is reorganized whenever some v.d() decreases
 * (key, value) = (v.d(), v)
 */
JRB initPriorityQueue(Graph graph, int start, double* d, int* path);

/** Free and repopulate the PQ with data from v.d() and graph*/
JRB updatePriorityQueue(Graph graph, JRB priorityQueue, double* d);

/** Void function of int*/
void void_funct_int(int v);
#endif