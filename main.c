#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "lib/ospf/ospf.h"

int main(){
  Network net = createNetwork();

  int op;
  int start_router, stop_router;
  double speed_demand;
  int prev[NETWORK_MAX_SIZE]; 

  do{
    printf("----------------------\n");
    printf("MENU\n");
    printf("1. Initialize network from file\n");
    printf("2. Simulate connection\n");
    printf("3. Show all forwarding table\n");
    printf("4. Show all link state table\n");
    printf("5. Find shortest path\n");
    printf("6. Find path with max capacity\n");
    printf("7. Modify network\n");

    do{
      printf("Enter: ");
      scanf(" %d", &op);
    }while(op> 8 || op<1);

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

      ConnectionList list = createConnection(net, start_router, stop_router, speed_demand);
      if(list == NULL) break;
      deactivateAllConnections(list, start_router, stop_router);
      printf("--------------------------\n");
      dropNetwork(net);
      break;

    case  3:
      showForwarding(net);
      break;

    case  4:
      showLinkState(net);
      break;

    case 5:
      printf("Choose start router:\n");
      scanf("%d", &start_router);
      printf("Choose stop router:\n");
      scanf("%d", &stop_router);
      double shortest_capacity = findShortestPath(net, start_router, stop_router, prev);
      if(shortest_capacity == INFINITY_LENGTH) printf("Path from %d to %d not found\n", start_router, stop_router);
      else printf("Shortest path from %d to %d found with capacity: %.2f Mbps\n", start_router, stop_router, shortest_capacity);
      break;

    case  6:
      printf("Choose start router:\n");
      scanf("%d", &start_router);
      printf("Choose stop router:\n");
      scanf("%d", &stop_router);
      
      double max_capacity = findMaxCapacityPath(net, start_router, stop_router, prev);

      if(max_capacity == INFINITY_LENGTH) printf("Path from %d to %d not found\n", start_router, stop_router);
      else printf("Max capacity path from %d to %d found with max_capacity: %.2f Mbps\n", start_router, stop_router, max_capacity);
      break;

    case  7:
      printf("Modified network");
      break;

    default:
      break;
    }
    
  }while(op != 4);
    
  return 0;
}

