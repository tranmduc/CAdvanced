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

    return net;
}

/** Tester return 1 if success, 0 if not */
int testNetwork(){
    Network net = initNetwork();
    dropNetwork(net);
    return 1;
}

int main(){
    int test = testNetwork();
    printf("%d\n", test);
}