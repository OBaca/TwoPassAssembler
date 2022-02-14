/*macros*/
#define COMMENT_CHECK \
    { \
        if(*linePointer == ';') \
            continue; \
    }



void manage_line(FILE*);

/* */
void manage(char *);

/*get_line, getting a line from a file */
void get_line(FILE*);

/*get_file, opens a file */
FILE* get_file(char *);

/*is_label, if there is label in a line and its valid we will return his name, if not return NULL */
char* is_label(char *, int *, int*, int);

/*add_label, adding a label to the symbol table with his values */
void add_label(char *, int); 

/*reading_data_line, reads a line, if there is errors it prints it out, and store data to the data_image */
void reading_data_line(char *, int *, int );

/*word_check, check if two strings are equal */
int word_check(char *, char *, int *);

/*IntCheck, check if a string of numbers is a valid int, and saves it. */
int intCheck(char *, int *, int *);

void reading_string_line(char *, int *, int );

char *createIntChar(char *, int);

int countNum(char *);


