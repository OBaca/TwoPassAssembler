
void manage_phaseTwo(FILE *,char *, head_of_data_lines* , head_of_symbol_list *, int *, int);


void create_entry_file(FILE *, char *, head_of_symbol_list*  ,char *, int *, int );


line *add_opcode_line(int );

void free_files_names(char *, char *, char *);

void read_op1_line(FILE * ,FILE *, head_of_symbol_list *, char *, int , int *, int );

void read_op2_line(FILE * ,FILE *, head_of_symbol_list *, char *, int , int *, int );

void print_to_external_file(FILE *,  symbol_table * );

symbol_table *symbol_compare_type1(head_of_symbol_list*  ,char *);

symbol_table *symbol_compare_type2(head_of_symbol_list*  ,char *,int *, int , int *);

void print_all_data_lines(FILE *,head_of_data_lines* );

