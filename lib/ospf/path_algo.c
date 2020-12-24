#include "ospf.h"
#include <stdio.h>

/******************************PATH ALGORITHMS***************************/
/** Implementation: path_algo.c */

/** Find the shortest path from start to stop and return its weakest link's speed
 * prev: int array pointer of the path (v1->v3->v2)
 * Return: the weakest link's speed in the path in Mbps. or INFINITY_LENGTH if no path is found
 * Usage: find path when create connection; find next hop
 * --------------------------
 * Assumption: non-negative weight edges
 * Algorithm: using Priority queue
 * For each vertex v in the graph, we maintain v.d(), the estimate of the shortest path from s, initialized to Infinity the start
 * Relaxing an edge (u,v) means testing whether we can improve the shortest path to v found so far by going through u
 * Priority queue is reorganized whenever some v.d() decreases
 *  */
double findShortestPath(Network network, int start, int stop, int* prev){
    if(checkExistance(network, start, stop) == 0) return INFINITY_LENGTH;

    double d[NETWORK_MAX_SIZE]; //estimation from start to every vertex

    JRB priorityQueue = initPriorityQueue(network, start, d, prev);

    //Pop from queue the smallest value and process
    while(!jrb_empty(priorityQueue)){
        //extract node with smalled d[]
        JRB min_node = jrb_first(priorityQueue);
        int vertex_min_in_PQ = jval_i(min_node->val);
        jrb_delete_node(min_node);
        //printf("findShortestPath - Ckpt 2: vertex_min_in_PQ is %d with d[] = %.2f\n", vertex_min_in_PQ, jval_d(min_node->key)); //DEBUG


        //Recalculate all path from vertex_min to outDegree
        int output[NETWORK_MAX_SIZE];
        int n = getAdjancentRouters(network, vertex_min_in_PQ, output);
        int current_des;

        for(int i=0; i<n; i++){
            current_des = output[i];
            relaxShortestPath(network, vertex_min_in_PQ, current_des, d, prev, priorityQueue);
        }
    }    
    

    //PQ is empty, spanning from start vertex complete
    for (size_t i = 1; i < 5; i++) printf("prev[%d] = %d\n", i, prev[i]); //DEBUG
    for (size_t i = 1; i < 5; i++)printf("d[%d] = %.2f\n", i, d[i]); //DEBUG

    //can not reach destination
    if (d[stop] == INFINITY_LENGTH) return INFINITY_LENGTH;

    //can reach destination, find path's capacity to return

    return findPathCapacity(network, start, stop, prev);  
}

/** Given a well-established path from start to stop, find its max capacity (or min(link's speed))
 * Return: minimum link speed of links on path (in Mbps); 0 if start == stop; Or NEGATIVE (-1) otherwise
*/
double findPathCapacity(Network network, int start, int stop, int* prev){
    if(checkExistance(network, start, stop) == 0) return NEGATIVE;
    if(start == stop) return 0;

    double minSpeed = INFINITY_LENGTH;
    int node1 = stop;
    int node2 = prev[stop];
    double linkSpeed;

    while(node1 != start){
        linkSpeed = getLinkSpeed(network, node2, node1);
        if(minSpeed > linkSpeed) minSpeed = linkSpeed;
        //move nodes forward
        node1 = node2;
        if(node1 != start) node2 = prev[node2];
    }
    return minSpeed;
}

/** Using findShortestPath() to find the next hop from start to stop(destination).
 * Return: id of the next hop router. Or -1 (NEGATIVE) if no path is available
*/
int findNextHop(Network network, int start, int stop){
    int path[NETWORK_MAX_SIZE];
    double shortestPath = findShortestPath(network, start, stop, path);
    if(shortestPath == INFINITY_LENGTH) return NEGATIVE;
    return path[start];
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
int relaxShortestPath(Network network, int intermediate_v, int current_des, double* d, int* prev, JRB priorityQueue){
    if(getLinkState(network, intermediate_v, current_des) != ACTIVE ) return 0; //only check for active links

    double new_weight = d[intermediate_v] + getLinkWeight(network, intermediate_v, current_des);

    printf("relax: inter:%d - des:%d - old weight: %.2f - new weight: %.2f\n", intermediate_v, current_des, d[current_des], new_weight); //DEBUG
    
    if(d[current_des] > new_weight){
        //it is better to go through intermediate_v to current_des
        //printf("Better\n"); //Relax
        d[current_des] = new_weight;
        prev[current_des] = intermediate_v; 
        updatePriorityQueue(priorityQueue, current_des , new_weight);
        return 1;
    }
    return 0;
}

/** Relax an edge to find the max capacity path */
int relaxMaxCapacity(Network network, int intermediate_v, int current_des, double* d, int* path, JRB priorityQueue){
    return 0;
}


/** Init the d,parent array and the priority queue
 * Priority queue is reorganized whenever some v.d() decreases
 * (key, value) = (v.d(), v)
 * Return: the JRB node of priority queue
 */
JRB initPriorityQueue(Network network, int start, double* d, int* prev){
    //init d[] and parent[]
    JRB router_node;
    jrb_traverse(router_node, network.router){
        int router = jval_i(router_node->key);
        d[router] = INFINITY_LENGTH;
        prev[router] = -1; //set parent of vertices to null
    }

    d[start] = 0;

    //a priority queue ADT keyed by v.d(), which is re-organized whenever some d decreases (v.d() is key of tree's node, v is value)
    return populatePriorityQueue(network, d);
}

/** Populate the PQ with data from v.d() and network.
 * Return: the JRB node of priority queue
*/
JRB populatePriorityQueue(Network network, double* d){
    JRB pq = make_jrb(); 
    //init priority queue (key, value) = (v.d, v) ~ (double, int)
    JRB router_node;
    jrb_traverse(router_node, network.router){
        int router = jval_i(router_node->key);
        jrb_insert_dbl(pq, d[router], new_jval_i(router));
    }

    return pq;
}

/** Update the priority queue due to the new d of vertex
 * Priority queue: to be updated
 * router: router id, used to find JRB node
 * new_d: new value of d
 * Algo: find the JRB node by router, then delete that node and add a new node (new_d, vertex)
 * Return: 1 if successful, 0 if vertex node not found
*/
int updatePriorityQueue(JRB priorityQueue, int router, double new_d){
    JRB node;
    jrb_traverse(node, priorityQueue){
        if(jval_i(node->val) == router){
            //found
            jrb_delete_node(node);
            jrb_insert_dbl(priorityQueue, new_d, new_jval_i(router));            
            return 1;
        }
    }

    //not found node
    return 0;
}