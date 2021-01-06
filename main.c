#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "lib/ospf/ospf.h"

int main(){
  Network net = createNetwork();

  int op;
  int start_router, stop_router;
  double speed_demand;
  char ip[50];
  int prev[NETWORK_MAX_SIZE]; 
  float speed;
  int link_state;

  do{
    printf("----------------------\n");
    printf("MENU\n");
    printf("1. Initialize network from file\n");
    printf("2. Simulate connection\n");
    printf("3. Show all forwarding table\n");
    printf("4. Show all link state table\n");
    printf("5. Find shortest path\n");
    printf("6. Find path with max capacity\n");
    printf("7. Add router\n");
    printf("8. Add connection\n");
    printf("9. Modify connection state\n");
    printf("10. Modify connection speed\n");
    printf("11. Exit\n");  
    do{
      printf("Enter: ");
      scanf(" %d", &op);
    }while(op> 11 || op<1);

    switch(op){
    case 1:
      importRouterFromFile(net, "./data/router.txt");
      importLinkFromFile(net, "./data/connection.txt");
      break;

    case  2:
      printf("Choose start router:\n");
      scanf("%d", &start_router); 
      printf("Choose stop router:\n");
      scanf("%d", &stop_router); 
      printf("Define speed demand:\n");
      scanf("%d", &speed_demand); 
      if(!hasRouter(net, start_router)){
        fprintf(stderr, "ERROR: The network has no router %d. Please add it first.\n", start_router);
        break;
      }
      if(!hasRouter(net, stop_router)){
          fprintf(stderr, "ERROR: The network has no router %d. Please add it first.\n", stop_router);
          break;
      }
      ConnectionList list = createConnection(net, start_router, stop_router, speed_demand);
      if(list == NULL) break;
      // deactivateAllConnections(list, start_router, stop_router);
      // dropNetwork(net);
      break;

    case  3:
      showLinkState(net);  
      break;

    case  4:
      showForwarding(net);
      break;

    case 5:
      printf("Choose start router:\n");
      scanf("%d", &start_router);
      printf("Choose stop router:\n");
      scanf("%d", &stop_router);
      if(!hasRouter(net, start_router)){
        fprintf(stderr, "ERROR: The network has no router %d. Please add it first.\n", start_router);
        break;
      }
      if(!hasRouter(net, stop_router)){
          fprintf(stderr, "ERROR: The network has no router %d. Please add it first.\n", stop_router);
          break;
      }
      double shortest_capacity = findShortestPath(net, start_router, stop_router, prev);
      if(shortest_capacity == INFINITY_LENGTH) printf("Path from %d to %d not found\n", start_router, stop_router);
      else printf("Shortest path from %d to %d found with capacity: %.2f Mbps\n", start_router, stop_router, shortest_capacity);
      break;

    case  6:
      printf("Choose start router:\n");
      scanf("%d", &start_router);
      printf("Choose stop router:\n");
      scanf("%d", &stop_router);
      if(!hasRouter(net, start_router)){
        fprintf(stderr, "ERROR: The network has no router %d. Please add it first.\n", start_router);
        break;
      }
      if(!hasRouter(net, stop_router)){
          fprintf(stderr, "ERROR: The network has no router %d. Please add it first.\n", stop_router);
          break;
      }
      double max_capacity = findMaxCapacityPath(net, start_router, stop_router, prev);

      if(max_capacity == INFINITY_LENGTH) printf("Path from %d to %d not found\n", start_router, stop_router);
      else printf("Max capacity path from %d to %d found with max_capacity: %.2f Mbps\n", start_router, stop_router, max_capacity);
      break;
    case  7:
      printf("Router IP:\n");
      scanf(" %s", ip);
      int next_router_ip = getNextRouterID(net);
      addRouter(net, next_router_ip, ip);

      break;
    case  8:
      printf("Input start router IP ID:\n");
      scanf("%d", &start_router);
      printf("Input stop router IP ID:\n");
      scanf("%d", &stop_router);
      printf("Input connection speed:\n");
      scanf("%f", &speed);
      printf("Input connection state:\n");
      scanf("%d", &link_state);
      if(!hasRouter(net, start_router)){
        fprintf(stderr, "ERROR: The network has no router %d. Please add it first.\n", start_router);
        break;
      }
      if(!hasRouter(net, stop_router)){
        fprintf(stderr, "ERROR: The network has no router %d. Please add it first.\n", stop_router);
        break;
      }
      if(hasLink(net, start_router, stop_router)){
        fprintf(stderr, "ERROR: Link from %d to %d already exists. Please use setLinkSpeed or setLinkState to modify.\n", start_router, stop_router);
        break;
      }
      addDirectedLink(net, start_router, stop_router, speed, link_state);
      break;
    case  9:
      printf("Choose start router:\n");
      scanf("%d", &start_router);
      printf("Choose stop router:\n");
      scanf("%d", &stop_router);
      printf("Choose new state:\n");
      scanf("%d", &link_state);
      if(!hasRouter(net, start_router)){
        fprintf(stderr, "ERROR: The network has no router %d. Please add it first.\n", start_router);
        break;
      }
      if(!hasRouter(net, stop_router)){
          fprintf(stderr, "ERROR: The network has no router %d. Please add it first.\n", stop_router);
          break;
      }
      if(!hasLink(net, start_router, stop_router)){
        fprintf(stderr, "ERROR: Link from %d to %d does not exist.\n", start_router, stop_router);
        break;
      }
      setLinkState(net, start_router,stop_router, link_state);
      break;
    case  10:
      printf("Input start router IP ID:\n");
      scanf("%d", &start_router);
      printf("Input stop router IP ID:\n");
      scanf("%d", &stop_router);
      printf("Input connection speed:\n");
      scanf("%f", &speed);
      double speed_db;
      speed_db = (double)speed;
      printf("Hello world");
      if(!hasRouter(net, start_router)){
        fprintf(stderr, "ERROR: The network has no router %d. Please add it first.\n", start_router);
        break;
      }
      if(!hasRouter(net, stop_router)){
          fprintf(stderr, "ERROR: The network has no router %d. Please add it first.\n", stop_router);
          break;
      }
      if(!hasLink(net, start_router, stop_router)){
        fprintf(stderr, "ERROR: Link from %d to %d does not exist.\n", start_router, stop_router);
        break;
      }
      setLinkSpeed(net, start_router,stop_router, speed_db);
      break;
    default:
      break;
    }
    
  }while(op != 11);
  dropNetwork(net);
  return 0;
}

