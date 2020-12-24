#include "ospf.h"
#include <stdio.h>

/******************************ROUTER***************************/
/** Implementation: router.c */

/** Add a router with an id and IP to the network.
 * Return: 1 if successful, 0 if error
 */
int addRouter(Network network, int id, char* IP){
    return 0;
}

/** Get a router JRB node through the id.
 * Return: JRB node if found. NULL if not.
 */
JRB getRouterbyID(Network network, int id){
    return NULL;
}

/** Get IP of a router through the id 
 * Return: the string of rounter's IP
*/
char* getRouterIPbyID(Network network, int id){
    return "";
}

/** Check if the network has vertex with id. Return: 1 (True) or 0 (False) */
int hasRouter(Network network, int id){
    return 0;
}

/** Remove a router along with its links.  Return: 1 (success) or 0 (fail)*/
int removeRouter(Network network, int id){
    return 0;
}