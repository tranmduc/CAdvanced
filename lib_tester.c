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

    addActiveLink(net, 1, 2, 40);
    addActiveLink(net, 1, 3, 5);
    addActiveLink(net, 1, 4, 6);
    addActiveLink(net, 2, 4, 9);

    return net;
}

/** Tester return 1 if success, 0 if not */
int testNetwork(){
    Network net = initNetwork();

    printf("Is 1 next to 2:%d\n", hasLink(net, 1, 2));
    printf("Is 1 next to 5:%d\n", hasLink(net, 1, 5));
    printf("Speed between 3 and 1: %.2f Mbps\n", getLinkSpeed(net, 3, 1));
    printf("Weight between 2 and 4: %.2f\n", getLinkWeight(net, 2, 4));
    removeLink(net, 1,2);
    printf("After removeLink, is 1 next to 2:%d\n", hasLink(net, 1, 2));
    addActiveLink(net, 1, 2, 20);
    printf("After re-add, weight between 1 and 2:%.2f\n", getLinkWeight(net, 1, 2));
    removeRouter(net, 4);
    printf("After remove rounter 4, is 4 next to 2:%d\n", hasLink(net, 4, 2));

    dropNetwork(net);
    return 1;
}

int main(){
    int testNet = testNetwork();
    printf("Pass testNetwork: %d\n", testNet);
}