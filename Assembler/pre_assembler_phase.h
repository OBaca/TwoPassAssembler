#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H

/*macro - is a function that recieve .as file and create new .am file with all the assignments of the macros*/
void macro(char *);

/*save_name - is a function that save string into a char pointer*/
char *save_name(char *);

/*macro_name_check - is a function that get two string and compare them. return 1 if they match and 0 otherwise*/
int macro_name_check(char *, char *, int *);

/*skipSpaceTab - is a function that skips spaces and tabs in a string*/
void skipSpaceTab(char *);

/*removeSpace - is a function that remove all the white spaces from the string*/
void removeSpace(char *);

/*transfer_macro - is a function that gets a pointer to a new file and a line that we check if there is a word that match to one of the saved macros, and a flag, if this is a macro that we saved we transfer the macro lines into the new .am file.  */
void transfer_macro(head_of_macro_list*,char [], FILE *, int *);

#endif
