

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
        struct symbol_table *next;
}symbol_table;


/*struct to store a data/string line  */
typedef struct line
{
	unsigned int classify:4;
	unsigned int signal:16;	
}line;

typedef struct head_of_list
{
        macro_table *head;
}head_of_list;

typedef struct head_of_symbol_list
{
        symbol_table* head;
}head_of_symbol_list;


/*create_list, creates a linked list */
void create_list();

/*add_data_parameter, adding a data line node */
line *add_data_parameter(int );

head_of_symbol_list *create_symbol_head();

void free_symbol_table_memory(head_of_symbol_list* );
