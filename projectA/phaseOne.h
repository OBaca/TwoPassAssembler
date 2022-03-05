/*macros*/
#define COMMENT_CHECK \
    { \
        if(*linePointer == ';') \
            continue; \
    }



void manage_phaseOne(FILE*, head_of_data_lines*, head_of_symbol_list*, int *);



/*get_line, getting a line from a file */
void get_line(FILE*);


/*is_label, if there is label in a line and its valid we will return his name, if not return NULL */
char* is_label(char *, int *, int*, int);

/*add_label, adding a label to the symbol table with his values */
void add_label(head_of_symbol_list* ,char *, int );

/*reading_data_line, reads a line, if there is errors it prints it out, and store data to the data_image */
void reading_data_line(head_of_data_lines*,char *, int *, int );

/*word_check, check if two strings are equal */
int word_check(char *, char *, int *);

/*IntCheck, check if a string of numbers is a valid int, and saves it. */
int intCheck(char *, int *, int *);

void reading_string_line(head_of_data_lines* ,char *, int *, int );

char *createIntChar(char *, int);

int countNum(char *);

void add_extern(head_of_symbol_list*  ,char *, int , int *, int );

void manage_code_lines(char *);

void skip_chars(char *);

/*for debugging please delete it later! !!@~!~!~!~!~!~!~~!~!~!~!~!!~!~~ */
void print_all_symbols(head_of_symbol_list* );
void print_data_lines_list(head_of_data_lines* );
/* !~!~!~!~!~!~~!~!~!~!~!!~!~!~!~!~!~!~!~~!~!~!~!~!!~!~!~!~!~!~!~!~~!~!~!~!~!!~!~*/


