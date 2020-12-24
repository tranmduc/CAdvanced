#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../standard/jrb.h"
#include "../standard/jrb.h"
// #include "ospf.h"

/*
    Read data from file network.txt 
*/
typedef struct{
    JRB Router;
    JRB Connection;
} Network;

Network createNetwork() {
    Network n;
    n.Router = make_jrb();
    n.Connection = make_jrb();
    return n;
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

int addConnection(Network network, int connection_id, int router_id_1, int router_id_2, float speed, int link_state)  {
    JRB node = jrb_find_int(network.Connection, connection_id);
    if(node == NULL) {
        jrb_insert_int(network.Connection, connection_id, new_jval_s(strdup(router_id_1))); 
        return 1;
    }  
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
int importConnectionFromFile(Network network,char* filename) {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) return 0;
    while((read = getline(&line, &len, fp)) != -1) {
        // printf("%s", line);
        int count = 0;
        char t_str[40];
        char connection_id[10],router_id_1[10],router_id_2[10],speed[10], link_state[10];

        strcpy(t_str, line);
        // printf("%s\n", t_str);
        char * pch = strtok(t_str, " ");
        while (pch != NULL)
        {
            if( count ==0) strcpy(connection_id, pch);
            else if (count ==1) strcpy(router_id_1, pch);
            else if (count ==2) strcpy(router_id_2, pch);
            else if (count ==3) strcpy(speed, pch);
            else if (count ==4) strcpy(link_state, pch);
            else break;
            pch = strtok (NULL, " ");
            count ++;
        }
        // printf("%s\n", router_id);
        // printf("%s\n", ip);
        int res = addConnection(network, atoi(connection_id), atoi(router_id_1),atoi(router_id_2),atof(speed),atoi(link_state));
        printf("%d\n", res);
    }
    fclose(fp);
    if (line) free(line);
    return 1;
}


int main(void) {
    Network network =createNetwork();
    int res = 0;
    res = importRouterFromFile(network, "../ioreader/router.txt");
}


