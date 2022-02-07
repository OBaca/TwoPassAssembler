FLAGS.C = gcc -c -Wall -ansi -pedantic

FLAGS.G = gcc -g -Wall -ansi -pedantic

HEADERS = constant.h macro.h inspectLine.h

assembler: macro.o inspectLine.o 
	$(FLAGS.G) macro.o inspectLine.o -o assembler


macro.o: macro.c $(HEADERS)
	$(FLAGS.C) macro.c -o macro.o

inspectLine.o: inspectLine.c $(HEADERS)
	$(FLAGS.C) inspectLine.c -o inspectLine.o


