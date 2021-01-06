#include "ospf.h"
#include "../standard/jrb.h"
#include <stdio.h>
/******************************LINK***************************/
/** Implementation: link.c */

/** Check link exists between r1 and r2. Return 1(True) or 0(False)*/
int hasLink(Network network, int router_1, int router_2){
    return (getLinkSpeed(network, router_1, router_2) != NEGATIVE );
}


/** Add a directed link from v1 to v2. Return 1 if success and 0 if not*/
int addDirectedLink(Network network, int router_1, int router_2, double speed, int state){    
    // ADDING: give graph has routers, and no links yet created

    ////JRB of router 1 in the network.linkState
    JRB router_1_jrb_speed = jrb_find_int(network.linkSpeed, router_1);
    JRB router_1_tree_speed;

    //add to linkSpeed
    if(router_1_jrb_speed == NULL){
        router_1_tree_speed = make_jrb(); //change from jrb->tree (node -> value)
        jrb_insert_int(network.linkSpeed, router_1, new_jval_v(router_1_tree_speed)); //add v1 tree to main tree
    } else{ //non empty
        router_1_tree_speed = (JRB) jval_v(router_1_jrb_speed->val);
    }
    
    jrb_insert_int(router_1_tree_speed, router_2, new_jval_d(speed)); //add v2 to v1's tree

    //add to linkState
    JRB router_1_jrb_state = jrb_find_int(network.linkState, router_1);
    JRB router_1_tree_state;
    //init new JRB for router 1 if empty
    if(router_1_jrb_state == NULL){
        router_1_tree_state = make_jrb(); //change from jrb->tree (node -> value)
        jrb_insert_int(network.linkState, router_1, new_jval_v(router_1_tree_state)); //add v1 tree to main tree
    } else{ //non empty
        router_1_tree_state = (JRB) jval_v(router_1_jrb_state->val);
    }
    
    jrb_insert_int(router_1_tree_state, router_2, new_jval_i(state)); //add v2 to v1's tree

    printf("Add link from %d to %d with speed %.2f and state %d sucessfully\n", router_1,router_2, speed, state); //DEBUG
    return 1;
    
}

/** Return 1 if network exists and has r1, r2; 0 otherwhise
 * Also printf(stderr) corresponding errors.
*/
int checkExistance(Network network, int router_1, int router_2){    
    if (network.router == NULL){
        fprintf(stderr, "ERROR: The network is empty. Please add router first before adding links\n");
        return 0;
    }
    //check if v1,v2 exists in graph.vertices
    if(!hasRouter(network, router_1)){
        fprintf(stderr, "ERROR: The network has no router %d. Please add it first.\n", router_1);
        printf("checkExistance: %d to %d\n", router_1, router_2);
        return 0;
    }
    if(!hasRouter(network, router_2)){
        fprintf(stderr, "ERROR: The network has no router %d. Please add it first.\n", router_2);
        printf("checkExistance: %d to %d\n", router_1, router_2);
        return 0;
    }
    return 1;
}
/** Add a link to network.
 * network: a network
 * router_1: id of 1st router
 * router_2: id of 2nd router
 * speed: the initial speed of the link in Mbps
 * state: is this link available (1-active or 0-inactive or 2-busy)
 * Return: 1 if success and 0 if not*/
int addLink(Network network, int router_1, int router_2, double speed, int state){
    if(checkExistance(network, router_1, router_2) == 0) return 0;

    //check if the edge already exists
    if(hasLink(network, router_1, router_2)){
        fprintf(stderr, "ERROR: Link from %d to %d already exists. Please us setLinkSpeed or setLinkState to modify.\n", router_1, router_2);
        return 0;
    }

    if (speed <=0 ){
        fprintf(stderr, "does not accept negative speed. Please try again\n");
        return 0;
    }

    //check state:
    switch (state){
    case ACTIVE:
    case BUSY:
    case INACTIVE:
        break;
    default:
        fprintf(stderr, "state argument with value %d unaccepted. Please try again\n", state);
        return 0;
    }

    //printf("Add link from %d to %d with speed %.2f and state %d sucessfully\n", router_1,router_2, speed, state); //DEBUG
    return addDirectedLink(network, router_1, router_2, speed, state) * addDirectedLink(network, router_2, router_1, speed, state);
}

/** Add an active link to network
 * network: a network
 * router_1: id of 1st router
 * router_2: id of 2nd router
 * speed: the initial speed of the link in Mbps
 * Return: 1 if success and 0 if not
*/
int addActiveLink(Network network, int router_1, int router_2, double speed){
    return addLink(network, router_1, router_2, speed, 1);
}

/**  Get weight between two routers. Used in Djikistra 
 * Return: weight in 100/speed or INFINITY_LENGTH if no link between r1 and r2*/
double getLinkWeight(Network network, int router_1, int router_2){
    double speed = getLinkSpeed(network, router_1, router_2);
    if(speed == NEGATIVE) return INFINITY_LENGTH;
    else return 100.0/speed;
}

/**  Get speed between two routers. 
 * Return: linkspeed in Mbps or NEGATIVE (-1) if no link between r1 and r2*/
double getLinkSpeed(Network network, int router_1, int router_2){
    if(checkExistance(network, router_1, router_2) == 0) return NEGATIVE;

    //check link
    JRB router_1_jrb_speed = jrb_find_int(network.linkSpeed, router_1);
    if(router_1_jrb_speed == NULL) return NEGATIVE;

    //has tree_speed, check if has router_2
    JRB router_1_tree_speed = (JRB) jval_v(router_1_jrb_speed->val);
    JRB speedJRB = jrb_find_int(router_1_tree_speed, router_2);
    if(speedJRB == NULL) return NEGATIVE;

    return jval_d(speedJRB->val);
}

/**  set speed between two routers. Auto-trigger update table
 * speed: in Mbps
 * Return: 1 (success) or 0 (fail)*/
int setLinkSpeed(Network network, int router_1, int router_2, double speed){

    if(getLinkSpeed(network, router_1, router_2) == -1) return 0;
    if (speed < 0 ){
        fprintf(stderr, "does not accept negative speed %.2f from %d to %d. Please try again\n", speed, router_1, router_2);
        return 0;
    }

    #define setLinkSpeedMacro(router_1, router_2)  {                        \             
        JRB router_1_jrb_speed = jrb_find_int(network.linkSpeed, router_1); \        
        JRB router_1_tree_speed = (JRB) jval_v(router_1_jrb_speed->val);    \      
        JRB speedJRB = jrb_find_int(router_1_tree_speed, router_2);         \     
        jrb_delete_node(speedJRB);                                          \     
        jrb_insert_int(router_1_tree_speed, router_2, new_jval_d(speed));   \                                                                       
    }               
    
    setLinkSpeedMacro(router_1, router_2);
    setLinkSpeedMacro(router_2, router_1);
                                                         
    #undef setLinkSpeedMacro
    printf("setLinkSpeed: %d to %d with speed %.2f\n", router_1, router_2, speed);
    return 1;
}

/**  Get state between two routers
 * Return: 1-active or 0-inactive or 2 (busy) or NEGATIVE (-1) if no link between r1 and r2*/
int getLinkState(Network network, int router_1, int router_2){
    if(checkExistance(network, router_1, router_2) == 0) return NEGATIVE;

    //check link
    JRB router_1_jrb_state = jrb_find_int(network.linkState, router_1);
    if(router_1_jrb_state == NULL) return NEGATIVE;

    //has tree_state, check if has router_2
    JRB router_1_tree_state = (JRB) jval_v(router_1_jrb_state->val);
    JRB stateJRB = jrb_find_int(router_1_tree_state, router_2);
    if(stateJRB == NULL) return NEGATIVE;

    return jval_i(stateJRB->val);

}

/**  set state of a link between two routers. Auto-trigger update table
 * state: either 0 (inactive) or 1 (active) or 2 (busy). other value not accepted.
 * Return: 1 (success) or 0 (fail)*/
int setLinkState(Network network, int router_1, int router_2, int state){

    if(checkExistance(network, router_1, router_2) == 0) return 0;
    //check state:
    switch (state){
    case ACTIVE:
    case BUSY:
    case INACTIVE:
        break;
    default:
        fprintf(stderr, "state argument with value %d unaccepted. Please try again\n", state);
        return 0;
    }

    #define setLinkStateMacro(router_1, router_2){                           \                                                    
        JRB router_1_jrb_state = jrb_find_int(network.linkState, router_1);  \
        if(router_1_jrb_state == NULL) return 0;                             \
        JRB router_1_tree_state = (JRB) jval_v(router_1_jrb_state->val);     \
        JRB stateJRB = jrb_find_int(router_1_tree_state, router_2);          \      
        if(stateJRB == NULL) return 0;                                       \     
        jrb_delete_node(stateJRB);                                           \   
        jrb_insert_int(router_1_tree_state, router_2, new_jval_i(state));    \
    } 

    setLinkStateMacro(router_1,router_2);
    setLinkStateMacro(router_2,router_1);
    #undef setLinkStateMacro;
    printf("setLinkState: %d to %d with state %d\n", router_1, router_2, state);
    return 1;

}

/** Remove linkState between r1 and r2. Return: 1 (success) or 0 (fail)*/
int removeLinkState(Network network, int router_1, int router_2){
    if(getLinkState(network, router_1, router_2) == NEGATIVE) return 0; //no link state

    #define removeLinkStateMacro(router_1, router_2){                           \    
        /* printf("Chkpt removeLinkStateMacro: %d, %d\n", router_1, router_2);*/\                                                  
        JRB router_1_jrb_state = jrb_find_int(network.linkState, router_1);     \
        if(router_1_jrb_state == NULL) return 0;                                \
        JRB router_1_tree_state = (JRB) jval_v(router_1_jrb_state->val);        \
        JRB stateJRB = jrb_find_int(router_1_tree_state, router_2);             \        
        /*printf("npath length %d to root: %d\n", router_2, jrb_plength(stateJRB)); */\  
        jrb_delete_node(stateJRB);                                              \ 
        if(jrb_empty(router_1_tree_state)){                      \         
            free(router_1_tree_state);                          \
            jrb_delete_node(router_1_jrb_state);                 \         
        }                                                       \
    } 

    removeLinkStateMacro(router_1,router_2);
    removeLinkStateMacro(router_2,router_1);
    #undef removeLinkStateMacro;

    return 1;
}
/** Remove linkSpeed between r1 and r2. Return: 1 (success) or 0 (fail)*/

int removeLinkSpeed(Network network, int router_1, int router_2){
    if(getLinkSpeed(network, router_1, router_2) == NEGATIVE) return 0; //no link speed

    #define removeLinkSpeedMacro(router_1, router_2)  {                        \      
        /* printf("Chkpt removeLinkSpeedMacro: %d, %d\n", router_1, router_2); */\       
        JRB router_1_jrb_speed = jrb_find_int(network.linkSpeed, router_1); \        
        JRB router_1_tree_speed = (JRB) jval_v(router_1_jrb_speed->val);    \      
        JRB speedJRB = jrb_find_int(router_1_tree_speed, router_2);         \
        jrb_delete_node(speedJRB);                                          \     
        if(jrb_empty(router_1_tree_speed)){                      \         
            free(router_1_tree_speed);                          \
            jrb_delete_node(router_1_jrb_speed);                 \         
        }                                                            \                         
    }                                                        \
    
    removeLinkSpeedMacro(router_1, router_2);
    removeLinkSpeedMacro(router_2, router_1);                                                         
    #undef removeLinkSpeedMacro

    return 1;
}
/** Remove link between r1 and r2 both speed and state. Return: 1 (success) or 0 (fail)*/
int removeLink(Network network, int router_1, int router_2){
    return removeLinkState(network, router_1, router_2) * removeLinkSpeed(network, router_1, router_2);
}

/** Count all rounters & links that connect to this vertex. Store list of vertices into output[NETWORK_MAX_SIZE]
 * router: rounter id
 * output[NETWORK_MAX_SIZE]: empty list to store the result
 * Return: number of routers, or NEGATIVE (-1) if failed.
*/
int getAdjancentRouters(Network network, int router, int output[NETWORK_MAX_SIZE]){
    JRB router_jrb = jrb_find_int(network.linkSpeed, router);
    if(router_jrb == NULL){
        return NEGATIVE;
    }

    JRB router_tree = (JRB) jval_v(router_jrb->val);
    JRB travelling_node;
    int index = 0;
    jrb_traverse(travelling_node, router_tree){
        output[index++] = jval_i(travelling_node->key);
        //speed = jval_i(travelling_node->val)
    }
    return index;
}