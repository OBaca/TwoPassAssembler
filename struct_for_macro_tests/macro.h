#include <stdio.h> /*delete later for debug */ 
#include <stdlib.h>
#include <string.h>

#define LENGTH_LINE 81
#define MAX_MACRO_LINE 6
typedef struct macros{
			char name[LENGTH_LINE];
			char macro_table[MAX_MACRO_LINE][LENGTH_LINE];
			struct macros *next;
		}ItemMacro;


typedef struct list{
		ItemMacro* head;
		int macro_count;
	}ItemList;



void macro(char *[]);

/*create_new_macro_file - is a function that gets a file name and change the ending of the name depends on the ending name given. */
char *create_new_macro_file(char *, char *, int);

int name_check(char *, char *, int *);
void skipSpaceTab(char *);
char *save_name(char *);
void removeSpace(char *);

int count_num_of_macros(FILE *);
void getFile(char *[]);

ItemList* createList();
void addToList(ItemList*, char[], char *[], int);

void printItem(ItemMacro *);
void printList(ItemList *);
