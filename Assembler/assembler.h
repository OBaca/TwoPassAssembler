#ifndef ASSEMBLER_H
#define ASSEMBLER_H

/*file_open, gets a file name, his ending, and determine his permissions and create a pointer to an existing file or create a new one. */
FILE *file_open(char *fileName, char *fileType, int amType);

/*manage, manage the phase one and phase two steps, and creates the needed files if there is not errors. */
void manage(char *);

#endif
