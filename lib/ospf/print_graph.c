#include "ospf.h"
#include "../standard/jrb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../standard/jval.h"


/** Given the network, print all routers & links */
void printNetwork(Network network){
    JRB node;
    int count = 0;

    printf("\nNetwork\n");

    jrb_traverse(node, network.router){
        count ++;
    }

    int* arr = malloc(count);
    int size = 0;

    jrb_traverse(node, network.router){
        arr[size] = jval_i(node->key);
        size++;
    }

    for(int i = 0; i < size; i++){
        char* ip1 = getRouterIPbyID(network, arr[i]);
        int link = 0;
        for(int j = 0; j < size; j++){
            if(hasLink(network, arr[i], arr[j])){
                link = 1;
                char* ip2 = getRouterIPbyID(network, arr[j]);
                int state = getLinkState(network, arr[i], arr[j]);
                printf("<IP%d - %s>: <IP%d - %s> <State - %d>\n", arr[i], ip1, arr[j], ip2, state);
            }
        }
        if(link ==0){
            printf("<IP%d - %s>: <NULL> <NULL>\n", arr[i], ip1);
        }   
    }

    free(arr);
    
    return;
}

/** Print rounters of a network
 * routers:
 * id 1: <IP1>
 * id 2: <IP2>
 * ...
*/
void printRouterNetwork(Network network){
    JRB node;
    printf("\nRouter Network\n");
    jrb_traverse(node, network.router){
        printf("ID %d: %s\n", jval_i(node->key), jval_s(node->val));
    }
    return;
}

/** Print links of a network
 * root
 * |- <IP1>: <IP2> <state>
 * |- <IP2>: <IP3> <state>
*/
void printLinkNetwork(Network network){
    JRB node;
    int count = 0;

    printf("\nLink Network\n");

    jrb_traverse(node, network.linkState){
        count ++;
    }

    int* arr = malloc(count);
    int size = 0;

    jrb_traverse(node, network.linkState){
        arr[size] = jval_i(node->key);
        size++;
    }

    for(int i = 0; i < size-1; i++){
        char* ip1 = getRouterIPbyID(network, arr[i]);
        for(int j = i+1; j < size; j++){
            if(hasLink(network, arr[i], arr[j])){
                char* ip2 = getRouterIPbyID(network, arr[j]);
                int state = getLinkState(network, arr[i], arr[j]);
                printf("<IP%d - %s>: <IP%d - %s> <State - %d>\n", arr[i], ip1, arr[j], ip2, state);
            }
        }
    }

    free(arr);
    
    return;
}