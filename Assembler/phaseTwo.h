#ifndef PHASE_TWO_H
#define PHASE_TWO_H

/*manage_phaseTwo, this function fully analyze the .am file and connect between the linked list of the symbol and data tables,
                    and print it out to an output files */
void manage_phaseTwo(essentials*,FILE *,char *, head_of_data_lines* , head_of_symbol_list *, int *);

/*print_all_data_lines, printing all the data lines to the object file */
void print_all_data_lines(essentials *,FILE *,head_of_data_lines* );

/*read_op1_line, analyze a line with one operand directive command */
void read_op1_line(essentials *,FILE * ,FILE *, head_of_symbol_list *, char *, int , int *, int );

/*read_op2_line, analyze a line with two operands directive command */
void read_op2_line(essentials* ,FILE * ,FILE *, head_of_symbol_list *, char *, int , int *, int );

/*symbol_compare_type1, compare between a name to the symbol list and check if the symbol is exist, if the symbol exist we return the pointer, if not return NULL */
symbol_table *symbol_compare_type1(head_of_symbol_list*  ,char *);

/*symbol_compare, compare between a name to the symbol list and check if the symbol is exist, if the symbol exist we return the pointer, if not return NULL 
                   also check the register number. type2 is a label with a register inside a square brackets */
symbol_table *symbol_compare_type2(head_of_symbol_list*  ,char *,int *, int , int *);

/*print_to_external_file, prints external label values to an external file */
void print_to_external_file(FILE *,  symbol_table *, int );

/*print_to_entry_file, prints entry label values to an entry file */
void print_to_entry_file(FILE *, head_of_symbol_list*  ,char *, int *, int );

#endif
