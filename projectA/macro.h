


/*macro - is a function that recieve .as file and create new .am file with all the assignments of the macros*/
void macro(char *);

/*create_new_macro_file - is a function that gets a file name and change the ending of the name depends on the ending name given. */
char *create_new_macro_file(char *, char *, int);

/*name_check - is a function that get two string and compare them. return 1 if they match and 0 otherwise*/
int name_check(char *, char *, int *);

/*skipSpaceTab - is a function that skips spaces and tabs in a string*/
void skipSpaceTab(char *);

/*save_name - is a function that save string into a char pointer*/
char *save_name(char *);

/*removeSpace - is a function that remove all the white spaces from the string*/
void removeSpace(char *);

/*add_node - is a function that insert a node(macro struct) to the end of the list*/
void add_node( char* name, char macro_table[6][81], int );

/*transfer_macro - is a function that gets a pointer to a new file and a line that we check if there is a word that match to one of the saved macros, and a flag, if this is a macro that we saved we transfer the macro lines into the new .am file.  */
void transfer_macro(char [], FILE *, int *);

/*free the memory that have been allocated */
void free_memory();


