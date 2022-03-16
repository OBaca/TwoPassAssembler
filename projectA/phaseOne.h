/*macros*/
#define COMMENT_CHECK \
    { \
        if(*linePointer == ';') \
            continue; \
    }

void manage_phaseOne(FILE*, head_of_data_lines*, head_of_symbol_list*, int *, int *, int *);

/*manage_code_lines, this functions update the IC depends on the number of lines needed for each line for a specific command. */
void manage_code_lines(char *);

/*skip_char, skipping chars in a line till the next white char */
void skip_chars(char *);

/*add_extern, add a symbol with extern symbol type to the symbol list */
void add_extern(head_of_symbol_list* ,char *, int , int *, int );

/*is_label, if there is label in a line and its valid we will return his name, if not return NULL */
char* is_label(char *, int *, int*, int);

/*word_check, check if two strings are equal */
int word_check(char *, char *, int *);

/*reading_data_line, reads a line, if there is an error it prints it out, and store data to the data_image */
void reading_data_line(head_of_data_lines*,char *, int *, int );

/*IntCheck, check if a string of numbers is a valid int, and saves it. */
int intCheck(char *, int *, int *);

/*createIntChar, convert an int to string */
char *createIntChar(char *, int);

/* countNum, calculate the length of the number and return it */
int countNum(char *);

/* reading_string_line, reads a line, if there is an error it prints it out, and store string values to the data_image */
void reading_string_line(head_of_data_lines* ,char *, int *, int );

/*get_line, getting a line from a file */
void get_line(FILE*);













