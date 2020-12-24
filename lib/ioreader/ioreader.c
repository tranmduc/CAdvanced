#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../standard/jrb.h"
// #include "ospf.h"

/*
    Read data from file network.txt 
*/
typedef struct{
    JRB Router;
    JRB LinkSpeed;
    JRB LinkState;
} Network;

Network createNetwork() {
    Network n;
    n.Router = make_jrb();
    n.LinkSpeed = make_jrb();
    n.LinkState = make_jrb();
    return n;
}

/** Check whether there's an edge from v1 to v2. Former: isAdjacent*/
int hasLink(Network network, int v1, int v2){
    if (network.LinkState == NULL || network.LinkState == NULL){
        return 0;
    }

    //non-empty graph
    JRB v1_main_tree_node = jrb_find_int(network.LinkState, v1);
    if(v1_main_tree_node == NULL){ //v1 not connected to any vertex
        return 0;
    }

    JRB v1_tree = (JRB) jval_v(v1_main_tree_node->val);
    JRB v2_node_on_v1_tree = jrb_find_int(v1_tree, v2);

    if(v2_node_on_v1_tree == NULL){
        return 0;
    } else {
        //printf("%d and %d are connected with weight %d\n", v1, v2, jval_i(v2_node_on_v1_tree->val));//DEBUG
        return 1;
    }
}

/** Check if the graph has vertex with id */
int hasRouter(Network network, int router_id){
    if (network.Router == NULL)
        return 0;
    //find
    JRB find_node = jrb_find_int(network.Router, router_id);
    if(find_node == NULL){
        return 0;
    }
    return 1;
}


/** Add a router with an id and IP to the network.
 * Return: 1 if successful, 0 if error
 */
int addRouter(Network network, int id, char* IP) {
    JRB node = jrb_find_int(network.Router, id);
    if(node == NULL) {
        jrb_insert_int(network.Router, id, new_jval_s(strdup(IP))); 
        return 1;
    }    
    else
        strcpy(jval_s(node->val), IP);
    return 0;
}

int addLinkSpeed(Network network, int router_id_1, int router_id_2, double speed)  {
    if (network.LinkSpeed == NULL){
        fprintf(stderr, "ERROR: The network is empty. Please add router first before adding link\n");
        return 0;
    }
    //check if v1,v2 exists in graph.vertices
    if(!hasRouter(network, router_id_1)){
        fprintf(stderr, "ERROR: The graph has no router %d. Please add it first.\n", router_id_1);
        return 0;
    }
    if(!hasRouter(network, router_id_2)){
        fprintf(stderr, "ERROR: The graph has no router %d. Please add it first.\n", router_id_2);
        return 0;
    }

    //check if the edge already exists
    if(hasLink(network, router_id_1, router_id_2)){
        fprintf(stderr, "ERROR: Edge from %d to %d already exists.\n", router_id_1, router_id_2);
        return 0;
    }

    JRB vertex_1_jrb = jrb_find_int(network.LinkSpeed, router_id_1);
    JRB vertex_1_tree;

    //init new JRB for vertex 1 if empty
    if(vertex_1_jrb == NULL) {
        vertex_1_tree = make_jrb(); //change from jrb->tree (node -> value)
        jrb_insert_int(network.LinkSpeed, router_id_1, new_jval_v(vertex_1_tree)); //add router_id_1 tree to main tree
    }  else {
        vertex_1_tree = (JRB) jval_v(vertex_1_jrb->val);
    }

    jrb_insert_int(vertex_1_tree, router_id_2, new_jval_d(speed)); //add v2 to v1's tree
    printf("Add edge from %d to %d with weight %.2f sucessfully\n", router_id_1,router_id_2, speed); //DEBUG
    return 1;
}


int addLinkState(Network network, int router_id_1, int router_id_2, int state)  {
    if (network.LinkState == NULL){
        fprintf(stderr, "ERROR: The network is empty. Please add router first before adding link\n");
        return 0;
    }
    //check if v1,v2 exists in graph.vertices
    if(!hasRouter(network, router_id_1)){
        fprintf(stderr, "ERROR: The graph has no router %d. Please add it first.\n", router_id_1);
        return 0;
    }
    if(!hasRouter(network, router_id_2)){
        fprintf(stderr, "ERROR: The graph has no router %d. Please add it first.\n", router_id_2);
        return 0;
    }

    //check if the edge already exists
    if(hasLink(network, router_id_1, router_id_2)){
        fprintf(stderr, "ERROR: Edge from %d to %d already exists.\n", router_id_1, router_id_2);
        return 0;
    }
    JRB vertex_1_jrb = jrb_find_int(network.LinkState, router_id_1);
    JRB vertex_1_tree;

    //init new JRB for vertex 1 if empty
    if(vertex_1_jrb == NULL) {
        vertex_1_tree = make_jrb(); //change from jrb->tree (node -> value)
        jrb_insert_int(network.LinkState, router_id_1, new_jval_v(vertex_1_tree)); //add router_id_1 tree to main tree
    }  else {
        vertex_1_tree = (JRB) jval_v(vertex_1_jrb->val);
    }

    jrb_insert_int(vertex_1_tree, router_id_2, new_jval_i(state)); //add v2 to v1's tree
    printf("Add edge from %d to %d with weight %.2d sucessfully\n", router_id_1,router_id_2, state); //DEBUG
    return 1;
}

/*
    Read data from file router.txt and add to router
    Step 1: Open file, read till EOF
    Step 2: While loop, add to router
*/
int importRouterFromFile(Network network,char* filename) {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) return 0;
    while((read = getline(&line, &len, fp)) != -1) {
        // printf("%s", line);
        int count = 0;
        char t_str[20];
        char router_id[10];
        char ip[10];

        strcpy(t_str, line);
        // printf("%s\n", t_str);
        char * pch = strtok(t_str, " ");
        while (pch != NULL)
        {
            if( count ==0) strcpy(router_id, pch);
            else if (count ==1) strcpy(ip, pch);
            else break;
            pch = strtok (NULL, " ");
            count ++;
        }
        // printf("%s\n", router_id);
        // printf("%s\n", ip);
        int res = addRouter(network, atoi(router_id), ip);
        printf("%d\n", res);
    }
    fclose(fp);
    if (line) free(line);
    return 1;
}

/*
    Read data from file connection.txt and add to router
    Step 1: Open file, read till EOF
    Step 2: While loop, add to router
*/
int importLinkFromFile(Network network,char* filename) {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) return 0;
    while((read = getline(&line, &len, fp)) != -1) {
        // printf("%s", line);
        int count = 0;
        char t_str[40];
        char router_id_1[10],router_id_2[10],speed[10], link_state[10];

        strcpy(t_str, line);
        // printf("%s\n", t_str);
        char * pch = strtok(t_str, " ");
        // split text by blank space
        while (pch != NULL)
        {
            if (count ==0) strcpy(router_id_1, pch);
            else if (count == 1) strcpy(router_id_2, pch);
            else if (count == 2) strcpy(speed, pch);
            else if (count == 3) strcpy(link_state, pch);
            else break;
            pch = strtok (NULL, " ");
            count ++;
        }
        int resSpeed = addLinkSpeed(network, atoi(router_id_1),atoi(router_id_2),atof(speed));
        int resState = addLinkState(network, atoi(router_id_1),atoi(router_id_2),atoi(link_state));

        printf("%d %d\n", resSpeed, resState);
    }
    fclose(fp);
    if (line) free(line);
    return 1;
}

int main(void) {
    Network network =createNetwork();
    int res = 0;
    res = importRouterFromFile(network, "router.txt");
    res = importLinkFromFile(network, "connection.txt");
}