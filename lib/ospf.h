#ifndef	_OSPF_H_
#define	_OSPF_H_

#include "jrb.h"

#define NETWORK_MAX_SIZE 100
#define INFINITY_LENGTH 999999
#define NEGATIVE -1

// Router states
#define ACTIVE 1
#define INACTIVE 0
#define BUSY 2 //used up all capacity

/** Network consists of a list of Router, list of speeds (1/weights) and lists of Link states (active or not) */
typedef struct{
    JRB Router;
    JRB LinkSpeed;
    JRB LinkState;
} Network;

/** Connection: from router_1 to router_2 with a speed demand (in Mbps)
 * Connection_id is needed because we may have more than 1 connection from r1 to r2. Id is auto-generated.
 * router1, router2: two end-points
 * path[] the path of this connection
 * Ex: <1, network_A, 4,5, 60> is connection with id 1, network A, from router 4 to 5 with a 60 Mbps speed.
*/

typedef struct{
    int id;
    Network network;
    int router1;
    int router2;
    double speed_demand;
    int path[NETWORK_MAX_SIZE];
} Connection; 

/** Double linked List of connections*/
typedef struct ConnectionNode{
    struct ConnectionNode *flink;
    struct ConnectionNode *blink;
    Connection val;
} *ConnectionList;

extern int id_generator = 1; //generate id

/******************************GENERAL***************************/
/** Implementation: network.c */

/** Initialization */
Network createNetwork();
Network importNetworkFromFile(char* filename);
/** Free the network*/
void dropNetwork(Network network);

/** Given the netowrk, print all rounters & links */
void printNetwork(Network network); 


/******************************PRINTING***************************/
/** Implementation: print_graph.c - Duc */

/** Print rounters of a network
 * routers:
 * id 1: <IP1>
 * id 2: <IP2>
 * ...
*/
void printRouterNetwork(Network network);

/** Print links of a network
 * root
 * |- <IP1>: <IP2> <active or not>
 * |- <IP2>: <IP3> <active or not>
*/
void printLinkNetwork(Network network);


/******************************ROUTER***************************/
/** Implementation: router.c */

/** Add a router with an id and IP to the network.
 * Return: 1 if successful, 0 if error
 */
int addRouter(Network network, int id, char* IP);

/** Get a router JRB node through the id.
 * Return: JRB node if found. NULL if not.
 */
JRB getRouterbyID(Network network, int id);

/** Get IP of a router through the id 
 * Return: the string of rounter's IP
*/
char* getRouterIPbyID(Network network, int id);

/** Check if the network has vertex with id. Return: 1 (True) or 0 (False) */
int hasRouter(Network network, int id);

/** Remove a router along with its links.  Return: 1 (success) or 0 (fail)*/
int removeRouter(Network network, int id);

/******************************LINK***************************/
/** Implementation: link.c */

/** Add a link to network.
 * network: a network
 * router_1: id of 1st router
 * router_2: id of 2nd router
 * speed: the initial speed of the link in Mbps
 * is_active: is this link available (1-active or 0-inactive)
 * Return: 1 if success and 0 if not*/
int addLink(Network network, int router_1, int router_2, double speed, int is_active);

/** Add an active link to network
 * network: a network
 * router_1: id of 1st router
 * router_2: id of 2nd router
 * speed: the initial speed of the link in Mbps
 * Return: 1 if success and 0 if not
*/
int addActiveLink(Network network, int router_1, int router_2, double speed);

/**  Get speed between two routers. 
 * Return: linkspeed in Mbps or NEGATIVE (-1) if no link between r1 and r2*/
double getLinkSpeed(Network network, int router_1, int router_2);

/**  set speed between two routers. Auto-trigger update table
 * speed: in Mbps
 * Return: 1 (success) or 0 (fail)*/
int setLinkSpeed(Network network, int router_1, int router_2, double speed);

/**  Get state between two routers
 * Return: 1-active or 0-inactive or 2 (busy) or NEGATIVE (-1) if no link between r1 and r2*/
int getLinkState(Network network, int router_1, int router_2);

/**  set state of a link between two routers. Auto-trigger update table
 * state: either 0 (inactive) or 1 (active) or 2 (busy). other value not accepted.
 * Return: 1 (success) or 0 (fail)*/
int setLinkState(Network network, int router_1, int router_2, int state);

/** Remove link between r1 and r2 both speed and state. Return: 1 (success) or 0 (fail)*/
int removeLink(Network network, int router_1, int router_2);

/** Count all rounters & links that connect to this vertex. Store list of vertices into output[NETWORK_MAX_SIZE]
 * router: rounter id
 * output[NETWORK_MAX_SIZE]: empty list to store the result
 * Return: number of routers, or NEGATIVE (-1) if failed.
*/
int getAdjancentRouters(Network network, int router, int output[NETWORK_MAX_SIZE]);



/******************************CONNECTION***************************/
/** Implementation: connection.c */

/** Start a list of connection from start to stop.
 * If speed_demand < capacity, then one connection is created only
 * If not, then the connection will be split into a list of Connection[]
 * Return: the list of Connection if sucess or NULL if failed
 * Algo:
 * Step 1: findShortestPath, if speed_demand is smaller thatn shortestPath's capacity, then choose this path
 * Step 2: if not, then findMaxCapacityPath. If return positive, OK; negative, to step 3
 * Step 3: Split connection into one connection with max_capacity speed demand is step 2.
 * Activate this connection (meaning modify all links' speed along the max_capacity path)
 * Go back to step 2 and try again with the new network
 * Terminate condition: speed_demand < capacity (success) or no more path can be created (failed)
*/
ConnectionList createConnection(Network network, int start, int stop, double speed_demand);

/** Start a connection. Modify the graph's edges
 * Return: 1 if sucess or 0 if failed
*/
int activateConnection(Connection *connection);

/** Stop a specific connection  (not delete it)
*/
int deactivateConnection(Connection *connection);

/** Stop all connections from start to stop (not delete it)
 * Return: 1 if sucess or 0 if failed
*/
int deactivateAllConnections(Network network, int start, int stop);



/******************************CONNECTION LIST***************************/
/** Implementation: dll_connection.c */

extern ConnectionList new_dllist();
extern void free_dllist(ConnectionList);

extern void dll_append(ConnectionList, Connection);
extern void dll_prepend(ConnectionList, Connection);
extern void dll_insert_b(ConnectionList, Connection);
extern void dll_insert_a(ConnectionList, Connection);

extern void dll_delete_node(ConnectionList);
extern int dll_is_empty(ConnectionList);

extern ConnectionList dll_find_node(ConnectionList, Connection);
extern ConnectionList dll_find_and_delete_node(ConnectionList, Connection);

extern Connection dll_val(ConnectionList);

#define dll_first(d) ((d)->flink)
#define dll_next(d) ((d)->flink)
#define dll_last(d) ((d)->blink)
#define dll_prev(d) ((d)->blink)
#define dll_nil(d) (d)

#define dll_traverse(ptr, list) \
  for (ptr = list->flink; ptr != list; ptr = ptr->flink)
#define dll_rtraverse(ptr, list) \
  for (ptr = list->blink; ptr != list; ptr = ptr->blink)


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
double findShortestPath(Network network, int start, int stop, int* path);

/** Using findShortestPath() to find the next hop from start to stop(destination).
 * Return: id of the next hop router. Or -1 (NEGATIVE) if no path is available
*/
int findNextHop(Network network, int start, int stop);


/** Find a path from start to stop with max_capacity, or max(min(speed)) along links in the path
 * Djkistra: path with weight 4-5-6 is better, which can hold 100/6=16,667 Mbps
 * Capacity: Path 5-5-5-5 is better, which can hold 20 Mbps
 * path: int array pointer of the path (v1->v3->v2)
 * Return: the max_capacity of a path which is < speed_demand, or -max_capacity if no path exceeds speed_demand
 * Or INFINITY_LENGTH if no path from start to stop can be found
 * Ex: Return 20 (success) or -20 (failed)
 * --------------------------
 * */
double findMaxCapacityPath(Network network, int start, int stop, double speed_demand, int* path);


/** Relaxing an edge (intermediate_v,current_des) means testing whether we can improve the shortest path to current_des found so far by going through intermediate_v 
 * d: array pointer of estimation shortest path from start
 * parent: use to backtrace the path
 * priorityQueue: update the PQ when relax successfully
 * Return: 1 - True if relaxed successfully, 0 - false otherwise
*/
int relax(Network network, int intermediate_v, int current_des, double* d, int* path, JRB priorityQueue);

/** Init the d,parent array and the priority queue
 * Priority queue is reorganized whenever some v.d() decreases
 * (key, value) = (v.d(), v)
 * Return: the JRB node of priority queue
 */
JRB initPriorityQueue(Network network, int start, double* d, int* path);

/** Populate the PQ with data from v.d() and network.
 * Return: the JRB node of priority queue
*/
JRB populatePriorityQueue(Network network, double* d);

/** Update the priority queue due to the new d of vertex
 * Priority queue: to be updated
 * router: router id, used to find JRB node
 * new_d: new value of d
 * Algo: find the JRB node by router, then delete that node and add a new node (new_d, vertex)
 * Return: 1 if successful, 0 if vertex node not found
*/
int updatePriorityQueue(JRB priorityQueue, int router, double new_d);

#endif