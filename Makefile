CC = gcc

all: lib_tester
	make clean

############### Without Libs
lib_tester: lib_tester.o jval.o jrb.o queue.o stack.o dllist.o jrb_graph_directed_weighted.o dllist.o
	${CC} $^ -o $@

lib_tester.o: lib_tester.c
	$(CC) -c $<

############### Libs 

input.o: lib/input.c 
	$(CC) -c $<

jrb.o: lib/jrb.c 
	$(CC) -c $<

jval.o: lib/jval.c 
	$(CC) -c $<

sort.o: lib/sort.c 
	$(CC) -c $<

jrb_graph_directed_weighted.o: lib/jrb_graph_directed_weighted.c
	$(CC) -c $<

ospf.o: lib/ospf.c dllist.o
	$(CC) -c $<
	
dllist.o: lib/dllist.c
	$(CC) -c $<

queue.o: lib/queue.c
	$(CC) -c $<

stack.o: lib/stack.c
	$(CC) -c $<
############### Others
.PHONY: clean
clean:
	rm -f *.o