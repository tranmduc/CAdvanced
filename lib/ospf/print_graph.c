#include "ospf.h"
#include "../standard/jrb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../standard/jval.h"


/** Given the network, print all routers & links */
void printNetwork(Network network){
    JRB node;
    // int count = 0;

    printf("\nNetwork\n");

    // jrb_traverse(node, network.router){
    //     count ++;
    // }

    // int* arr = malloc(count);
    int arr[NETWORK_MAX_SIZE];
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

    // free(arr);
    
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

    // int* arr = malloc(count);
    int arr[NETWORK_MAX_SIZE];
    int size = 0;

    jrb_traverse(node, network.router){
        arr[size] = jval_i(node->key);
        size++;
    }

    for(int i = 0; i < count; i++){
        char* ip1 = getRouterIPbyID(network, arr[i]);
        for(int j = 0; j < size; j++){
            if(hasLink(network, arr[i], arr[j])){
                char* ip2 = getRouterIPbyID(network, arr[j]);
                int state = getLinkState(network, arr[i], arr[j]);
                printf("<IP%d - %s>: <IP%d - %s> <State - %d> \n", arr[i], ip1, arr[j], ip2, state);
            }
        }
    }
    return;
}

/*Show link state table */
void showLinkState(Network network){
    printLinkNetwork(network);
}

/*Show forwarding table */
void showForwarding(Network network){
    JRB node;

    int arr[NETWORK_MAX_SIZE];
    int size = 0;

    jrb_traverse(node, network.router){
        arr[size] = jval_i(node->key);
        size++;
    }
    printf("\nForwarding Table Network\n");
    jrb_traverse(node, network.router){
        int stop = jval_i(node->key);
        for(int i = 0; i < size; i++){
            if(stop != arr[i]){
                int start = arr[i];
                int nextHop = findNextHop(network, start, stop);
                char* startIp = getRouterIPbyID(network, start);
                if(nextHop != -1){
                    printf("<Des: IP%d> <Src: IP%d - %s> <Next hop: %d>\n", stop, start, startIp, nextHop);
                }else{
                    printf("<Des: IP%d> <Src: IP%d - %s> <Next hop: Not Available>\n", stop, start, startIp);
                }               
            }
        }
        
    }
    return;
}