FLAGS.C = gcc -c -Wall -ansi -pedantic

FLAGS.G = gcc -g -Wall -ansi -pedantic

HEADERS = constant.h macro.h inspectLine.h errors.h data_structure.h

assembler: macro.o inspectLine.o errors.o
	$(FLAGS.G) macro.o inspectLine.o errors.o -o assembler


macro.o: macro.c $(HEADERS)
	$(FLAGS.C) macro.c -o macro.o

inspectLine.o: inspectLine.c $(HEADERS)
	$(FLAGS.C) inspectLine.c -o inspectLine.o

errors.o: errors.c $(HEADERS)
	$(FLAGS.C) errors.c -o errors.o


