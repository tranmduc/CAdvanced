#include "ospf.h"
#include <stdio.h>

/******************************PATH ALGORITHMS***************************/
/** Implementation: path_algo.c */

/** Find the shortest path from start to stop and return its weakest link's speed
 * path: int array pointer of the path (v1->v3->v2)
 * Return: the weakest link's speed in the path in Mbps. or INFINITY_LENGTH if no path is found
 * Usage: find path when create connection; find next hop
 * --------------------------
 * Assumption: non-negative weight edges
 * Algorithm: using Priority queue
 * For each vertex v in the graph, we maintain v.d(), the estimate of the shortest path from s, initialized to Infinity the start
 * Relaxing an edge (u,v) means testing whether we can improve the shortest path to v found so far by going through u
 * Priority queue is reorganized whenever some v.d() decreases
 *  */
double findShortestPath(Network network, int start, int stop, int* path){
    return INFINITY_LENGTH;
}

/** Using findShortestPath() to find the next hop from start to stop(destination).
 * Return: id of the next hop router. Or -1 (NEGATIVE) if no path is available
*/
int findNextHop(Network network, int start, int stop){
    return NEGATIVE;
}


/** Find a path from start to stop with max_capacity, or max(min(speed)) along links in the path
 * Djkistra: path with weight 4-5-6 is better, which can hold 100/6=16,667 Mbps
 * Capacity: Path 5-5-5-5 is better, which can hold 20 Mbps
 * path: int array pointer of the path (v1->v3->v2)
 * Return: the max_capacity of a path which is < speed_demand, or -max_capacity if no path exceeds speed_demand
 * Or INFINITY_LENGTH if no path from start to stop can be found
 * Ex: Return 20 (success) or -20 (failed)
 * --------------------------
 * */
double findMaxCapacityPath(Network network, int start, int stop, double speed_demand, int* path){
    return INFINITY_LENGTH;
}


/** Relaxing an edge (intermediate_v,current_des) means testing whether we can improve the shortest path to current_des found so far by going through intermediate_v 
 * d: array pointer of estimation shortest path from start
 * parent: use to backtrace the path
 * priorityQueue: update the PQ when relax successfully
 * Return: 1 - True if relaxed successfully, 0 - false otherwise
*/
int relax(Network network, int intermediate_v, int current_des, double* d, int* path, JRB priorityQueue){
    return 0;
}

/** Init the d,parent array and the priority queue
 * Priority queue is reorganized whenever some v.d() decreases
 * (key, value) = (v.d(), v)
 * Return: the JRB node of priority queue
 */
JRB initPriorityQueue(Network network, int start, double* d, int* path){
    return NULL;
}

/** Populate the PQ with data from v.d() and network.
 * Return: the JRB node of priority queue
*/
JRB populatePriorityQueue(Network network, double* d){
    return NULL;
}

/** Update the priority queue due to the new d of vertex
 * Priority queue: to be updated
 * router: router id, used to find JRB node
 * new_d: new value of d
 * Algo: find the JRB node by router, then delete that node and add a new node (new_d, vertex)
 * Return: 1 if successful, 0 if vertex node not found
*/
int updatePriorityQueue(JRB priorityQueue, int router, double new_d){
    return 0;
}