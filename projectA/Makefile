FLAGS.C = gcc -c -Wall -ansi -pedantic

FLAGS.G = gcc -g -Wall -ansi -pedantic

HEADERS = constant.h macro.h phaseOne.h phaseTwo.h errors.h data_structure.h assembler.h

assembler: assembler.o macro.o phaseOne.o phaseTwo.o errors.o
	$(FLAGS.G) assembler.o macro.o phaseOne.o phaseTwo.o errors.o -o assembler

assembler.o: assembler.c $(HEADERS)
	$(FLAGS.C) assembler.c -o assembler.o

macro.o: macro.c $(HEADERS)
	$(FLAGS.C) macro.c -o macro.o

phaseOne.o: phaseOne.c $(HEADERS)
	$(FLAGS.C) phaseOne.c -o phaseOne.o

phaseTwo.o: phaseTwo.c $(HEADERS)
	$(FLAGS.C) phaseTwo.c -o phaseTwo.o

errors.o: errors.c $(HEADERS)
	$(FLAGS.C) errors.c -o errors.o


