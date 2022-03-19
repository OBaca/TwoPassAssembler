FLAGS.C = gcc -c -Wall -ansi -pedantic 

FLAGS.G = gcc -g -Wall -ansi -pedantic 

OBJECTS = assembler.o pre_assembler_phase.o phaseOne.o phaseTwo.o manage_line.o errors.o data_structure.o

HEADERS = constant.h pre_assembler_phase.h phaseOne.h phaseTwo.h manage_line.h errors.h data_structure.h assembler.h

assembler: $(OBJECTS)
	$(FLAGS.G) $(OBJECTS) -lm -o assembler

assembler.o: assembler.c $(HEADERS)
	$(FLAGS.C) assembler.c -o assembler.o

data_structure.o: data_structure.c $(HEADERS)
	$(FLAGS.C) data_structure.c -o data_structure.o

macro.o: pre_assembler_phase.c $(HEADERS)
	$(FLAGS.C) pre_assembler_phase.c -o pre_assembler_phase.o

phaseOne.o: phaseOne.c $(HEADERS)
	$(FLAGS.C) phaseOne.c -o phaseOne.o

phaseTwo.o: phaseTwo.c $(HEADERS)
	$(FLAGS.C) phaseTwo.c -o phaseTwo.o

manage_line2.o: manage_line.c $(HEADERS) 
	$(FLAGS.C) manage_line.c -lm -o manage_line.o

errors.o: errors.c $(HEADERS)
	$(FLAGS.C) errors.c -o errors.o



