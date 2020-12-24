#include "ospf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../standard/jval.h"
#include "../standard/jrb.h"


/******************************GENERAL***************************/
/** Implementation: network.c */

/** Initialization */
Network createNetwork(){
    Network net;
    net.router = make_jrb();
    net.linkSpeed = make_jrb();
    net.linkState = make_jrb();
    return net;
}

/** Free the network*/
void dropNetwork(Network network){
    JRB node;
    jrb_traverse(node, network.linkSpeed){
        jrb_free_tree((JRB) jval_v(node->val));
    }

    jrb_traverse(node, network.linkState){
        jrb_free_tree((JRB) jval_v(node->val));
    }
         
    jrb_free_tree(network.router);
    jrb_free_tree(network.linkSpeed);
    jrb_free_tree(network.linkState);
    return;
}
