#include <stdio.h> /*delete later for debug */ 
#include <stdlib.h>
#include <string.h>

#define LENGTH_LINE 81
#define LENGTH_MACRO 6

/*define of struct node to store a macro name and his lines*/
typedef struct node
{
        char macro_name[LENGTH_LINE];
        char macro_lines[LENGTH_MACRO][LENGTH_LINE];
        struct node *next;
}node;

/*macro - is a function that recieve .as file and create new .am file with all the assignments of the macros*/
void macro(char *[]);

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

/*insert_end - is a function that insert a node(macro struct) to the end of the list*/
void insert_end(node** root, node* new_node, char* name, char macro_table[6][81], int i);
