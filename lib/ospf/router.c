#include "ospf.h"
#include <stdio.h>

/******************************ROUTER***************************/
/** Implementation: router.c */

/** Add a router with an id and IP to the network.
 * Return: 1 if successful, 0 if error
 */
int addRouter(Network network, int id, char* IP){
    //Check for duplicate id
    JRB node = jrb_find_int(network.router, id);

    if(node != NULL){
        //error, return
        fprintf(stderr, "ERROR: Router with id %d already exists. Cannot add another one.\n", id);
        return;
    }

    //key not found, create new key
    JRB new_node = jrb_insert_int(network.router, id, new_jval_s(IP));
    printf("Add router %d with name %s to network successfully\n", id, jval_s(new_node->val));
    return;
}

/** Get a router JRB node through the id.
 * Return: JRB node if found. NULL if not.
 */
JRB getRouterbyID(Network network, int id){
    return jrb_find_int(network.router, id);
}

/** Get IP of a router through the id 
 * Return: the string of rounter's IP
*/
char* getRouterIPbyID(Network network, int id){
    JRB found = getRouterbyID(network, id);
    if (found == NULL){
        fprintf(stderr, "ERROR: Router with id %d does not exist. Cannot get its IP.\n", id);
        return "";
    }
    return jval_s(found->val);
}

/** Check if the network has vertex with id. Return: 1 (True) or 0 (False) */
int hasRouter(Network network, int id){
    return (getRouterbyID(network, id) != NULL);
}

/** Remove a router along with its links & states.  Return: 1 (success) or 0 (fail)*/
int removeRouter(Network network, int id){
    JRB router = jrb_find_int(network.router, id);
    if(router == NULL){
        fprintf(stderr, "ERROR: Router with id %d does not exist. Cannot remove it.\n", id);
        return 0;
    }

    //found
    int adjacent_routers[NETWORK_MAX_SIZE];
    int n = getAdjancentRouters(network, id, adjacent_routers);
    if(n != NEGATIVE){
        //remove links to adj rounters
        for (int i = 0; i < n; i++)
        {
            removeLink(network, id, adjacent_routers[i]);
        }
        
    }

    //remove router in network.router
    jrb_delete_node(router);
}