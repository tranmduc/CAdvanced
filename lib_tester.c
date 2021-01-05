//Testing the lib's important function

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "lib/ospf/ospf.h"


Network initNetwork(){
   Network net = createNetwork();
   char str[10];

   addRouter(net, 1, "192.168.1.1");
   addRouter(net, 2, "192.168.1.2");
   addRouter(net, 3, "192.168.1.3");
   addRouter(net, 4, "192.168.1.4");
   addRouter(net, 5, "192.168.1.5");
   addRouter(net, 6, "192.168.1.6");

   addActiveLink(net, 1, 2, 4.0);
   addActiveLink(net, 2, 3, 4.0);
   addActiveLink(net, 3, 4, 4.0);
   addActiveLink(net, 1, 5, 3.0);
   addActiveLink(net, 4, 5, 3.0);

   return net;
}

/** Tester return 1 if success, 0 if not */
int testNetwork(){
   Network net = initNetwork();

   printf("Is 1 next to 2:%d\n", hasLink(net, 1, 2));
   printf("Is 1 next to 5:%d\n", hasLink(net, 1, 5));

   if(getLinkSpeed(net, 3, 1) == -1) printf("No link between 3 and 1\n");
   else printf("Speed between 3 and 1: %.2f Mbps\n", getLinkSpeed(net, 3, 1));

   if(getLinkWeight(net,2,4) == INFINITY_LENGTH) printf("No link between 2 and 4\n");
   else printf("Weight between 2 and 4: %.2f\n", getLinkWeight(net, 2, 4));

   removeLink(net, 1,2);
   printf("After removeLink 1-2, is 1 next to 2:%d\n", hasLink(net, 1, 2));
   addActiveLink(net, 1, 2, 20);
   printf("After re-add, weight between 1 and 2:%.2f\n", getLinkWeight(net, 1, 2));
   removeRouter(net, 4);
   printf("After remove rounter 4, is 4 next to 3:%d\n", hasLink(net, 4, 3));

   dropNetwork(net);
   return 1;
}

int testShortestPath(){
   Network net = initNetwork();

   int start = 1;
   int stop = 4;
   int prev[NETWORK_MAX_SIZE];

   double capacity = findShortestPath(net, start, stop, prev);

   if(capacity == INFINITY_LENGTH) printf("Path from %d to %d not found\n", start, stop);
   else printf("Shortest path from %d to %d found with capacity: %.2f Mbps\n", start, stop, capacity);

   dropNetwork(net);
   return 1;
}

int testMaxCap(){
   Network net = initNetwork();

   int start = 1;
   int stop = 4;
   int prev[NETWORK_MAX_SIZE];

   double capacity = findMaxCapacityPath(net, start, stop, prev);

   if(capacity == INFINITY_LENGTH) printf("Path from %d to %d not found\n", start, stop);
   else printf("Max capacity path from %d to %d found with capacity: %.2f Mbps\n", start, stop, capacity);

   dropNetwork(net);
   return 1;
}

int testSimulate(Network net){
   // Network net = initNetwork();
   int start = 1;
   int stop = 5;

   //Three scenarios: first is shortest path, second is using two connections, third is not enough cap
   double speed_demand1 = 2;
   double speed_demand2 = 4;
   double speed_demand3 = 8;


   ConnectionList list = createConnection(net, start, stop, speed_demand1);
   if(list == NULL) return 0;
   deactivateAllConnections(list, start, stop);
   printf("--------------------------\n");
   ConnectionList list2 = createConnection(net, start, stop, speed_demand2);
   if(list2 == NULL) return 0;
   deactivateAllConnections(list2, start, stop);
   printf("--------------------------\n");
   ConnectionList list3 = createConnection(net, start, stop, speed_demand3);
   if(list3 != NULL) return 0;

   // dropNetwork(net);

   return 1;
}

int testPrint(){
   Network net = initNetwork();
   printRouterNetwork(net);
   printLinkNetwork(net);
   printNetwork(net);
   return 1;
}

Network testReadFile() {
    Network net = createNetwork();
    importRouterFromFile(net, "./data/router.txt");
    importLinkFromFile(net, "./data/connection.txt");
   //  printRouterNetwork(net);
   //  printLinkNetwork(net);
   //  printNetwork(net);
    return net;
}

int testShowLinkStateTable(Network net){
   // Network net = initNetwork();
   showLinkState(net);
}

int testShowForwardingTable(Network net){
   // Network net = initNetwork();
   showForwarding(net);
}

void testSimulation(Network net, int start, int stop, double speed_demand){
   simulate(net, start, stop, speed_demand);
}

int main(){
   Network net = testReadFile();
   // printf("Pass testNetwork: %d\n", testNetwork());
   // printf("Pass testShortestPath: %d\n", testShortestPath());
   // printf("Pass testMaxCap: %d\n", testMaxCap());
   // printf("Pass testSimulate: %d\n", testSimulate());
   testSimulation(net, 2, 1, 2);
}
