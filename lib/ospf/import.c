#include "ospf.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/******************************IMPORT***************************/
/** Implementation: import.c - Hung */
 Network importNetworkFromFile(Network network, char* filename){
     char *line = NULL;
     size_t len = 0;
     ssize_t read;

     FILE *fp = fopen(filename, "r");
     if (fp == NULL) return network;
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
     return network;
 }

 Network importLinkFromFile(Network network, char  *filename) {
     char *line = NULL;
     size_t len = 0;
     ssize_t read;

     FILE *fp = fopen(filename, "r");
     if (fp == NULL) return network;
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
         addDirectedLink(network, atoi(router_id_1),atoi(router_id_2), atof(speed),atoi(link_state));
     }
     fclose(fp);
     if (line) free(line);
     return network;
 }