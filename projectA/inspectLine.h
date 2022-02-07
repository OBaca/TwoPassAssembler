
#define COMMENT_CHECK \
    { \
        if(*linePointer == ';') \
            continue; \
    }


void manage_line(FILE*);

void manage(char *);


void get_line(FILE*);

FILE* get_file(char *);

char* is_label(char *, int *, int*, int);

void manage_data_directive();

void add_label(char *, int);

void reading_data_line(char * );

int word_check(char *, char *, int *);
