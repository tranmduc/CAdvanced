#include "ospf.h"
/******************************LINK***************************/
/** Implementation: link.c */

/** Add a link to network.
 * network: a network
 * router_1: id of 1st router
 * router_2: id of 2nd router
 * speed: the initial speed of the link in Mbps
 * is_active: is this link available (1-active or 0-inactive)
 * Return: 1 if success and 0 if not*/
int addLink(Network network, int router_1, int router_2, double speed, int is_active){
    return 0;
}

/** Add an active link to network
 * network: a network
 * router_1: id of 1st router
 * router_2: id of 2nd router
 * speed: the initial speed of the link in Mbps
 * Return: 1 if success and 0 if not
*/
int addActiveLink(Network network, int router_1, int router_2, double speed){
    return 0;
}

/**  Get weight between two routers. Used in Djikistra 
 * Return: weight in 100/speed or INFINITY_LENGTH if no link between r1 and r2*/
double getLinkWeight(Network network, int router_1, int router_2){
    return INFINITY_LENGTH;
}

/**  Get speed between two routers. 
 * Return: linkspeed in Mbps or NEGATIVE (-1) if no link between r1 and r2*/
double getLinkSpeed(Network network, int router_1, int router_2){
    return NEGATIVE;
}

/**  set speed between two routers. Auto-trigger update table
 * speed: in Mbps
 * Return: 1 (success) or 0 (fail)*/
int setLinkSpeed(Network network, int router_1, int router_2, double speed){
    return 0;
}

/**  Get state between two routers
 * Return: 1-active or 0-inactive or 2 (busy) or NEGATIVE (-1) if no link between r1 and r2*/
int getLinkState(Network network, int router_1, int router_2){
    return NEGATIVE;
}

/**  set state of a link between two routers. Auto-trigger update table
 * state: either 0 (inactive) or 1 (active) or 2 (busy). other value not accepted.
 * Return: 1 (success) or 0 (fail)*/
int setLinkState(Network network, int router_1, int router_2, int state){
    return 0;
}

/** Remove link between r1 and r2 both speed and state. Return: 1 (success) or 0 (fail)*/
int removeLink(Network network, int router_1, int router_2){
    return 0;
}

/** Count all rounters & links that connect to this vertex. Store list of vertices into output[NETWORK_MAX_SIZE]
 * router: rounter id
 * output[NETWORK_MAX_SIZE]: empty list to store the result
 * Return: number of routers, or NEGATIVE (-1) if failed.
*/
int getAdjancentRouters(Network network, int router, int output[NETWORK_MAX_SIZE]){
    return NEGATIVE;
}