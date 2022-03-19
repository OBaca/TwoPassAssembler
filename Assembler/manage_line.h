#ifndef MANAGE_LINE_H
#define MANAGE_LINE_H

/*insert_claassify, turning on the classify flag. */
void insert_classify(int, line *);

/*insert_funct, insert the funct value to the structure */
void insert_funct(int, line *);

/*insert_register, insert the register value to the structure depends if its a source or destination */
void insert_register(int, int, line *);

/*insert_address, insert the adress value to the structure depends if its a source or destination */
void insert_address(int ,int, line *);

/*insert_opcode, gets an opcode number and turns on the flag on the opcode line */
void insert_opcode(int , line *);

/*print_obj_line, gets a line and print it out to the object file in the desired format */
void print_obj_line(FILE *,line *, int );

#endif
