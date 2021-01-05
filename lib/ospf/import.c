#include "ospf.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/******************************IMPORT***************************/
/** Implementation: import.c - Hung */
int importRouterFromFile(Network network, char* filename){
     char *line = NULL;
     size_t len = 0;
     ssize_t read;

     FILE *fp = fopen(filename, "r");
     if (fp == NULL) return 0;
     while((read = getline(&line, &len, fp)) != -1) {
         // printf("%s", line);
         int count = 0;
         char t_str[30];
         char router_id[10];
         char ip[20];

         strcpy(t_str, line);
         // printf("%s\n", t_str);
         char * pch = strtok(t_str, " ");
         while (pch != NULL)
         {
             if( count ==0) strcpy(router_id, pch);
             else if (count ==1) {
                 char * pch_ = strtok(pch, "\n");
                 strcpy(ip, pch_);
              }
             else break;
             pch = strtok (NULL, " ");
             count ++;
         }
        //  printf("%s\n", router_id);
        //  printf("%s\n", ip);
         addRouter(network, atoi(router_id), ip);
        //  printf("%d\n", res);
        // free(t_str);
        // free(router_id);
        // free(ip);
     }
     fclose(fp);
     if (line) free(line);
     return 1;
 }

int importLinkFromFile(Network network, char  *filename) {
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
             else if (count == 3) {
                 char * pch_ = strtok(pch, "\n");
                 strcpy(link_state, pch_);
             }
             else break;
             pch = strtok (NULL, " ");
             count ++;
         }
         addDirectedLink(network, atoi(router_id_1),atoi(router_id_2), atof(speed), atoi(link_state) );
     }
     fclose(fp);
     if (line) free(line);
     return 1;
 }