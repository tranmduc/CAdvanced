#include "ospf.h"


//Demo implementation & Return value of the OBPF headers

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