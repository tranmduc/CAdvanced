##Author: Nguyen Minh Tu - 20154203

CC = gcc

all: lib_tester
	make clean

############### Without Libs
lib_tester: lib_tester.o jval.o jrb.o queue.o stack.o dllist.o jrb_graph_directed_weighted.o dllist.o
	${CC} $^ -o $@

lib_tester.o: lib_tester.c
	$(CC) -c $<

############### Libs 
jrb.o: lib/standard/jrb.c 
	$(CC) -c $<

jval.o: lib/standard/jval.c 
	$(CC) -c $<

sort.o: lib/standard/sort.c 
	$(CC) -c $<

jrb_graph_directed_weighted.o: lib/standard/jrb_graph_directed_weighted.c
	$(CC) -c $<

ospf.o: lib/ospf/ospf.c dllist.o
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