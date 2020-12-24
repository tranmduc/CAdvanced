#include "ospf.h";
#include<stdio.h>;

/******************************CONNECTION***************************/
/** Implementation: connection.c */
//TODO: Implement this lib

extern int id_generator = 1; //generate id

/** Start a list of connection from start to stop - mimicking packet switching
 * If speed_demand < capacity, then one connection is created only
 * If not, then the connection will be split into a list of Connection[]
 * Return: the list of Connection if sucess or NULL if failed
 * ------------------
 * Algo:
 * Step 1: findShortestPath, if speed_demand is smaller thatn shortestPath's capacity, then choose this path
 * Step 2: if not, then findMaxCapacityPath. If return positive, OK; negative, to step 3
 * Step 3: Split connection into one connection with max_capacity speed demand is step 2.
 * Activate this connection (meaning modify all links' speed along the max_capacity path)
 * Go back to step 2 and try again with the new network
 * Terminate condition: speed_demand < capacity (success) or no more path can be created (failed)
*/
ConnectionList createConnection(Network network, int start, int stop, double speed_demand){
    return NULL;
}

/** Start a connection. Modify the graph's edges
 * Return: 1 if sucess or 0 if failed
*/
int activateConnection(Connection *connection){
    return 0;
}

/** Stop a specific connection  (not delete it)
 * Return: 1 if sucess or 0 if failed */
int deactivateConnection(Connection *connection){
    return 0;
}

/** Stop all connections from start to stop (not delete it)
 * Return: 1 if sucess or 0 if failed
*/
int deactivateAllConnections(Network network, int start, int stop){
    return 0;
}