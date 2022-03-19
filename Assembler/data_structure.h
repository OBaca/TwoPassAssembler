#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

/*define of struct node to store a macro name and his lines*/
typedef struct macro_table
{
        char macro_name[LENGTH_LINE];
        char macro_lines[LENGTH_MACRO][LENGTH_LINE];
        int line_max_indx;
        struct macro_table *next;
}macro_table;


/*define of struct node to store data in the symbol table*/
typedef struct symbol_table
{
        char symbol[LENGTH_LABEL];
        int value;
        int base_address;
        int offset;
        char attributes[LENGTH_ATTRIBUTES];
        int printed;
        struct symbol_table *next;
}symbol_table;


/*struct to store a line  */
typedef struct line
{
	unsigned int classify:4;
	unsigned int signal:16;	
}line;

/*struct to create a node from muliple data lines */
typedef struct data_lines
{
        line *data;
        struct data_lines *next;
}data_lines;

/*struct of the head of macro list */
typedef struct head_of_macro_list
{
        macro_table *head;
}head_of_macro_list;

/*struct of the head of symbol list */
typedef struct head_of_symbol_list
{
        symbol_table* head;
}head_of_symbol_list;

/*struct of the head of data list */
typedef struct head_of_data_lines
{
        data_lines *head;
}head_of_data_lines;

/*struct of essentials for the IC and DC. */
typedef struct essentials
{
        int IC;
        int TIC;
        int DC;
}essentials;


/*create_macro_list, creates a linked list of macro table */
head_of_macro_list *create_macro_list(head_of_macro_list*);

/*create_symbol_list, create a linked list of symbol table */
head_of_symbol_list *create_symbol_list();

/*create_data_list, create a linked list of a data symbol table */
head_of_data_lines *create_data_list();

/*add_data_parameter, adding a data line node */
line *add_data_parameter(int );

/*create_single_line, create a single 20bytes line */
line *create_single_line();

/*add_opcode_line, creates an opcode line with his opcode number */
line *add_opcode_line(int );

/*add_macro - adding a macro to the macro table with his values*/
void add_macro(head_of_macro_list*,char* name, char macro_table[6][81], int );

/*add_label, adding a label to the symbol table with his values */
void add_label(head_of_symbol_list* ,char *, int , int TIC);

/*add_data_line, adding a data line to the data table with his values */
void add_data_line(head_of_data_lines*  , line *);

/*free_macro_table_memory, free the memory that have been allocated in the macro table */
void free_macro_table_memory(head_of_macro_list*);

/*free_symbol_table_memory, free the memory that have been allocated in the symbol table */
void free_symbol_table_memory(head_of_symbol_list* );

/*free_data_line_memory, free the memory that have been allocated in the data table */
void free_data_line_memory(head_of_data_lines* );

/*free_files_names, free all the file names memory that have been allocated */
void free_files_names(char *, char *, char *);

/*create_essentials, create an essentials menu an initialize it, contains the IC and DC paramters and command names */
essentials *create_essentials();

#endif
