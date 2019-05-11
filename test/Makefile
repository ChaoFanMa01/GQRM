prefix=../src/
objects=graph.o $(prefix)graph.o $(prefix)node.o $(prefix)coordinate.o\
        $(prefix)prr.o $(prefix)random.o $(prefix)single_linked_list.o
std=-std=c99
graph: $(objects)
	gcc $(objects) $(std) -lm -o graph
graph.o: graph.c $(prefix)graph.h
	gcc $(std) -c graph.c
$(prefix)graph.o: $(prefix)graph.c $(prefix)graph.h
	gcc $(std) -c $(prefix)graph.c
$(prefix)node.o: $(prefix)node.c $(prefix)node.h
	gcc $(std) -c $(prefix)node.c
$(prefix)coordinate.o: $(prefix)coordinate.c $(prefix)coordinate.h
	gcc $(std) -c $(prefix)coordinate.c
$(prefix)prr.o: $(prefix)prr.c $(prefix)prr.h
	gcc $(std) -c $(prefix)prr.c
$(prefix)random.o: $(prefix)random.c $(prefix)random.h
	gcc $(std) -c $(prefix)random.c
$(prefix)single_linked_list.o: $(prefix)single_linked_list.c\
                      $(prefix)single_linked_list.h
	gcc $(std) -c $(prefix)single_linked_list.c
.PYTHON: clean
clean:
	-rm graph $(objects)
