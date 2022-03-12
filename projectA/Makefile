FLAGS.C = gcc -c -Wall -ansi -pedantic

FLAGS.G = gcc -g -Wall -ansi -pedantic

OBJECTS = assembler.o macro.o phaseOne.o phaseTwo.o manage_line2.o errors.o

HEADERS = constant.h macro.h phaseOne.h phaseTwo.h manage_line2.h errors.h data_structure.h assembler.h

assembler: $(OBJECTS)
	$(FLAGS.G) $(OBJECTS) -lm -o assembler

assembler.o: assembler.c $(HEADERS)
	$(FLAGS.C) assembler.c -o assembler.o

macro.o: macro.c $(HEADERS)
	$(FLAGS.C) macro.c -o macro.o

phaseOne.o: phaseOne.c $(HEADERS)
	$(FLAGS.C) phaseOne.c -o phaseOne.o

phaseTwo.o: phaseTwo.c $(HEADERS)
	$(FLAGS.C) phaseTwo.c -o phaseTwo.o

manage_line2.o: manage_line2.c $(HEADERS) 
	$(FLAGS.C) manage_line2.c -lm -o manage_line2.o

errors.o: errors.c $(HEADERS)
	$(FLAGS.C) errors.c -o errors.o



