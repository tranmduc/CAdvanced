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

/** Start a connection. Modify the graph's linkSpeed & linkState.
 * Given: all graph's links are active and has enough capacity
 * Return: 1 if sucess or 0 if failed
*/
int activateConnection(Connection connection){
    printf("Activating the connection #%d from router %d to %d with speed demand %.2f\n", connection.id,
        connection.router1, connection.router2, connection.speed_demand);
    
    int stop_router = connection.router2;
    int start_router = connection.prev[stop_router];
    int ret = 1; //return value. False if any setLink fails

    do{
        //modify speed & state
        double old_speed = getLinkSpeed(connection.network, start_router, stop_router);
        double new_speed = old_speed - connection.speed_demand;

        if(new_speed < 0){
            fprintf(stderr, "ERROR: speed_demand > capacity for connection %d\n", connection.id);
            return 0;
        } else{
            if(new_speed == 0){
                printf("Used up capacity of link %d to %d. Change linkState to BUSY\n", start_router, stop_router);
                ret *= setLinkState(connection.network, start_router, stop_router, BUSY);
            }
            ret *= setLinkSpeed(connection.network, start_router, stop_router, new_speed);
        }
        
        //traverse
        stop_router = connection.prev[stop_router];
        start_router = connection.prev[start_router];
    } while (start_router != connection.router1);
    
    return ret;
}

/** Stop a specific connection  (not delete it)
 * Return: 1 if sucess or 0 if failed */
int deactivateConnection(Connection connection){
    printf("Deactivating the connection #%d from router %d to %d with speed demand %.2f\n", connection.id,
        connection.router1, connection.router2, connection.speed_demand);
    
    int stop_router = connection.router2;
    int start_router = connection.prev[stop_router];
    int ret = 1; //return value. False if any setLink fails

    do{
        //modify speed & state
        double old_speed = getLinkSpeed(connection.network, start_router, stop_router);
        double new_speed = old_speed + connection.speed_demand;

        if(old_speed == 0){
            printf("Change linkState %d to %d to ACTIVE\n", start_router, stop_router);
            ret *= setLinkState(connection.network, start_router, stop_router, ACTIVE);
        }
        ret *= setLinkSpeed(connection.network, start_router, stop_router, new_speed);
        
        //traverse
        stop_router = connection.prev[stop_router];
        start_router = connection.prev[start_router];
    } while (start_router != connection.router1);
    
    return ret;
}

/** Stop all connections from start to stop & delete them
 * list: the connectionList from which connections are fetched & deleted
 * start, stop: endpoints of connections
 * Return: 1 if sucess or 0 if failed
*/
int deactivateAllConnections(ConnectionList list, int start, int stop){
    int ret = 1;
    ConnectionList node = dll_find_next_connection_by_start_stop(list, start, stop);
    while(node != NULL){
        ret *= deactivateConnection(node->val);
        //next node
        node = dll_find_next_connection_by_start_stop(list, start, stop);
    }
    return ret;
}