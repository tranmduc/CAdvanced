#include "ospf.h";
#include<stdio.h>;
#include<stdlib.h>;

/******************************CONNECTION***************************/
/** Implementation: connection.c */
//TODO: Implement this lib

extern int id_generator = 1; //generate id

/** Return a connection object with:
 * auto-incremental id
*/
Connection initConnection(Network network, int router1, int router2, double speed_demand, int* prev){
    Connection con;
    con.id = id_generator++;
    con.network = network;
    con.router1 = router1;
    con.router2 = router2;
    con.speed_demand = speed_demand;

    //copy content of prev
    int node = router2;
    printf("connectionPath: %d",node);

    while(node != router1){
        con.prev[node] = prev[node];
        node = prev[node];
        printf("<-%d",node);
    }   
    printf("\n");

    //printf("Con #%d prev pointer addr: %.p\n", con.id, prev);
    return con;
}

/** Start a list of connection from start to stop - mimicking packet switching
 * If speed_demand < capacity, then one connection is created only
 * If not, then the connection will be split into a list of Connection[] (implemented by dll - double linked list)
 * Return: the list of Connection if sucess or NULL if failed
 * ------------------
 * Algo:
 * Step 1: findShortestPath, if speed_demand is smaller than shortestPath's capacity, then choose this path
 * Step 2: if not, then findMaxCapacityPath. If return positive, OK; negative, to step 3
 * Step 3: Split connection into one connection with max_capacity speed demand is step 2.
 * Activate this connection (meaning modify all links' speed along the max_capacity path)
 * Go back to step 2 and try again with the new network
 * Terminate condition: speed_demand < capacity (success) or no more path can be created (failed)
*/
ConnectionList createConnection(Network network, int start, int stop, double speed_demand){
    if(checkExistance(network, start, stop) == 0) return NULL;
    if(speed_demand <= 0){
        fprintf(stderr, "Err: Speed demand must be positive (in Mbps)\n");
        return NULL;
    }
    ConnectionList return_list = new_dllist();

    //Step 1: shortest path
    int prev[NETWORK_MAX_SIZE];
    double shortest_path_capacity = findShortestPath(network, start, stop, prev);
    if(shortest_path_capacity == INFINITY_LENGTH){
        fprintf(stderr, "Err: No path from %d to %d can be found. Cannot create connections.\n", start, stop);
        return NULL;
    }

    //printf("createConnection: Ckpt 1\n");

    if(speed_demand <= shortest_path_capacity){
        //printf("createConnection: Ckpt 2.1 - SPF, speed_demand: %.2f, shortestCap: %.2f\n", speed_demand, shortest_path_capacity);

        //choose this path, activate connection and return
        Connection con = initConnection(network, start, stop, speed_demand, prev);
        if( activateConnection(con) == 1){ //activate successfully
            dll_append(return_list, con);
            printf("Simulate connection %d to %d successfully\n", start, stop);
            return return_list;
        } else return NULL;
    } else{

        //printf("createConnection: Ckpt 2.2 - maxCap\n");
        //Step 2: findMaxCapacityPath
        double maxCapacity = 0;

        while( (speed_demand > 0) && (maxCapacity != INFINITY_LENGTH) ){
            int prev2[NETWORK_MAX_SIZE];

            //printf("createConnection: Ckpt 3.1 - speed_demand: %.2f, start %d stop %d\n", speed_demand, start, stop);

            maxCapacity = findMaxCapacityPath(network, start, stop, prev2);
            if(maxCapacity == INFINITY_LENGTH) break;

            //printf("createConnection: Ckpt 3.2 - maxcap: %.2f\n", maxCapacity);
            
            double con_speed = (speed_demand < maxCapacity) ? speed_demand : maxCapacity; //min

            Connection con = initConnection(network, start, stop, con_speed, prev2);

            //printf("createConnection: Ckpt 3.3 - con %d inited\n", con.id);

            activateConnection(con);

            //printf("createConnection: Ckpt 3.4 - con %d activated %d\n", con.id, activateConnection(con));

            dll_append(return_list, con);
            if (maxCapacity != INFINITY_LENGTH) speed_demand -= con_speed;

        }

        //terminate:
        
        //network has enough/not enough capacity to simulate connection
        if (maxCapacity == INFINITY_LENGTH){
            //printf("createConnection: Ckpt 4.1 - not enough for speed_demand %.2f\n", speed_demand);   
            fprintf(stderr, "Err: This network does not have enough capacity to hold this connection\n");
            deactivateAllConnections(return_list, start, stop);
            return NULL;
        } else{
            //printf("createConnection: Ckpt 4.2 - enough for speed_demand %.2f\n", speed_demand);  
            printf("Simulate connections %d to %d successfully\n", start, stop);      
            return return_list;
        }
        
    }
    
}

/** Start a connection. Modify the graph's linkSpeed & linkState.
 * Given: all graph's links are active and has enough capacity
 * Return: 1 if sucess or 0 if failed
*/
int activateConnection(Connection connection){
    printf("Activating the connection #%d from router %d to %d with speed demand %.2f\n", connection.id,
        connection.router1, connection.router2, connection.speed_demand);
    
    int node1 = connection.router2;
    int node2 = connection.prev[node1];
    int ret = 1; //return value. False if any setLink fails

    while (node1 != connection.router1){
        //modify speed & state
        //printf("activateConnection: start %d to stop %d\n", node2, node1); //DEBUG

        double old_speed = getLinkSpeed(connection.network, node2, node1);
        double new_speed = old_speed - connection.speed_demand;

        if(new_speed < 0){
            fprintf(stderr, "ERROR: speed_demand > capacity for connection %d\n", connection.id);
            return 0;
        } else{
            if(new_speed == 0){
                printf("Used up capacity of link %d to %d. Change linkState to BUSY\n", node2, node1);
                ret *= setLinkState(connection.network, node2, node1, BUSY);
                //printf("activateConnection: Link state %d to %d set to BUSY\n", node2, node1);
            }
            ret *= setLinkSpeed(connection.network, node2, node1, new_speed);
            //printf("activateConnection: Link speed %d to %d set to %.2f\n", node2, node1, new_speed);
        }
        
        //traverse
        node1 = node2;
        if(node1 != connection.router1) node2 = connection.prev[node2];
    }

    //printf("activateConnection: Out loop, ret=%d\n", ret);
    
    return ret;
}


/** Stop a specific connection  (not delete it)
 * Return: 1 if sucess or 0 if failed */
int deactivateConnection(Connection connection){
    printf("Deactivating the connection #%d from router %d to %d with speed demand %.2f\n", connection.id,
        connection.router1, connection.router2, connection.speed_demand);
    
    int node1 = connection.router2;
    int node2 = connection.prev[node1];
    int ret = 1; //return value. False if any setLink fails

    while (node1 != connection.router1){
        //printf("deactivateConnection: start %d to stop %d\n", node2, node1); //DEBUG

        //modify speed & state
        double old_speed = getLinkSpeed(connection.network, node2, node1);
        double new_speed = old_speed + connection.speed_demand;

        if(old_speed == 0){
            printf("Change linkState %d to %d back to ACTIVE\n", node2, node1);
            ret *= setLinkState(connection.network, node2, node1, ACTIVE);
        }
        ret *= setLinkSpeed(connection.network, node2, node1, new_speed);
        
        //traverse
        node1 = node2;
        if(node1 != connection.router1) node2 = connection.prev[node2];
    }
    
    //printf("deactivateConnection: Out loop, ret=%d\n", ret);

    return ret;
}

/** Stop all connections from start to stop & delete them
 * list: the connectionList from which connections are fetched & deleted
 * start, stop: endpoints of connections
 * Return: 1 if sucess or 0 if failed
*/
int deactivateAllConnections(ConnectionList list, int start, int stop){
    if(dll_is_empty(list)){
        fprintf(stderr, "Err: ConnectionList is empty\n");
        return 0;
    }

    int ret = 1;
    ConnectionList node = dll_find_next_connection_by_start_stop(list, start, stop);
    while(node != NULL){
        ret *= deactivateConnection(node->val);
        dll_delete_node(node);
        //next node
        node = dll_find_next_connection_by_start_stop(list, start, stop);
    }

    //printf("deactiveAll: out loop, ret=%d\n", ret);
    return ret;
}

void simulate(Network net, int start, int stop, double speed_demand){
    ConnectionList list = createConnection(net, start, stop, speed_demand);
    if(list == NULL) return;
}