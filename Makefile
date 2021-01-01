##Author: Nguyen Minh Tu - 20154203

CC = gcc

all: lib_tester
	make clean

############### Executable
lib_tester: lib_tester.o network.o connection.o dll_connection.o link.o import.o print_graph.o path_algo.o router.o jrb.o jval.o
	${CC} $^ -o $@

lib_tester.o: lib_tester.c
	$(CC) -c $<

############### OSPF Libs 
network.o: lib/ospf/network.c
	$(CC) -c $<

connection.o: lib/ospf/connection.c
	$(CC) -c $<

dll_connection.o: lib/ospf/dll_connection.c
	$(CC) -c $<

link.o: lib/ospf/link.c
	$(CC) -c $<

print_graph.o: lib/ospf/print_graph.c
	$(CC) -c $<

path_algo.o: lib/ospf/path_algo.c
	$(CC) -c $<

router.o: lib/ospf/router.c
	$(CC) -c $<	

import.o: lib/ospf/import.c
	$(CC) -c $<	

############### Standard Libs
jrb.o: lib/standard/jrb.c 
	$(CC) -c $<

jval.o: lib/standard/jval.c 
	$(CC) -c $<

sort.o: lib/standard/sort.c 
	$(CC) -c $<


dllist.o: lib/standard/dllist.c
	$(CC) -c $<

queue.o: lib/standard/queue.c
	$(CC) -c $<

stack.o: lib/standard/stack.c
	$(CC) -c $<
############### Others
.PHONY: clean
clean:
	rm -f *.o