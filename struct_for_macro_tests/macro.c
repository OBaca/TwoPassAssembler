#include "macro.h"

int main(int argc, char *argv[])
{
	macro(argv);
	return 0;
}

/*This function gets a FILE POINTER ! , DONT FORGET TO CHANGE $$ */
void macro(char *argv[])
{
	FILE *fp, *newfp; /*fp pointer to .as, newfp pointer to .am */
	int nameSize = strlen(argv[1]); /*the length of the file name */
	char line[LENGTH_LINE]; /*to store each line in the file */
	char *linePointer; 
	int lettersCounter=0;
	char *macro_table[LENGTH_LINE]; /*macro table - to store macro name and the lines for the macro */
	int indx_table=0;
	int macro_signal=0; /*flag to know if we are inside a macro */
	char *macro_name;
	ItemList *list = createList();
	int lol;

	int counter = 0;


	/*open file named .as and point to it.  */
	if(!(fp= fopen(create_new_macro_file(argv[1], ".as",nameSize ), "r"))){
		printf("\nError: cannot open file name: %s\n",argv[1]); 
		return;
	}
	
	/*create new file named .am */
	if(!(newfp = fopen(create_new_macro_file(argv[1], ".am", nameSize), "w+")))
	{
		printf("\nError: cannot create a file name: %s.am\n",argv[1]); 
		return;
	}	

	/*reading through the lines in the file */
	while(fgets(line,LENGTH_LINE , fp) != NULL)	
	{
		counter++;
		printf("\nline text:%d\n",counter);
		
		linePointer = line;
		if(!macro_signal){
			
			/*skipping tabs and spaces */
			skipSpaceTab(linePointer);

			/*if the next char is not the beginning of "macro" we will copy the whole line to the new file  */
			if(*(linePointer) != 'm')
			{
				fputs(line, newfp);
				continue;
			}
			
			/*The current char must be 'm' */
			/*we check if the word is "macro", if not we will copy the whole line to the new file */
			if(!( name_check(linePointer, "macro", &lettersCounter) ))
			{
				fputs(line, newfp);
				continue;
			}
			/*now the word must be macro so we updating the pointer to be after macro name. */
			linePointer += lettersCounter;
			lettersCounter=0; /*reset the counter for future use. */
					
			/*if the next char is not a space, we will copy the whole line to the new file */
			if(*(linePointer) != ' ')
			{
				fputs(line, newfp);
				continue;
			}
			
			/*skipping spaces and tabs */
			skipSpaceTab(linePointer);

			/*if the next char is the end of the line, the name is not defined, so we copy the whole line to the new file*/
			if(*(linePointer) == '\n' )
			{
				fputs(line, newfp);
				continue;
			}
			
			/*saving the name of the macro */
			macro_name = save_name(linePointer);
			macro_signal = 1;
			
		}
		else
		{
			
			/*we are inside the box of macro */
			/*skipping spaces and tabs */
			printf("\nline: %s,  line 96\n", linePointer);
			skipSpaceTab(linePointer);
			printf("\nline: %s,  line 98\n", linePointer);
			lettersCounter =0;
			
			if(!( name_check(linePointer, "endm", &lettersCounter) ))
			{
				printf("line 104");
				
				macro_table[indx_table++] = line;
				printf("\n macro:%s  ,indx: %d",macro_table[indx_table - 1], indx_table-1);
				continue;
			}
			printf("FINALLY");
			lettersCounter=0;
			macro_signal = 0;
			macro_table[indx_table] = '\0';

			printf("\nline115 =  macro:%s",macro_table[0]);
			printf("\n macro:%s",macro_table[1]);
			printf("\n macro:%s",macro_table[2]);			

			

			/*add it to the list */
			addToList(list,macro_name, macro_table, indx_table);
			printf("line 110");
			indx_table=0;
			
		}
		
	}
		printList(list);
	
}


char * save_name(char *linePointer)
{
	int j;
	int name_length = 0;
	char *name;
	for(j=0 ; linePointer[j] && linePointer[j] != '\n' && linePointer[j] != ' ' && linePointer[j] != '\t'; j++ )
	{
		name_length++;
	}

	name = (char *)malloc(sizeof(char) * (name_length+1));

	for(j=0; j < name_length ; j++)
	{
		name[j] = linePointer[j];
	}
	name[++j] = '\0';
	return name;
}

int name_check(char *linePointer, char *name, int *lettersCounter)
{
	int j=0;
	
	while((linePointer[j] >= 'A' && linePointer[j] <= 'Z') || (linePointer[j] >= 'a' && linePointer[j] <= 'z') || (linePointer[j] >= 0 && linePointer[j] <= 9) || linePointer[j] == '#' )
	{
		(*lettersCounter)++;
		j++;
	}
	printf("\nline 152: %d ,%d\n", strlen(name), *lettersCounter);
	if(strlen(name) < (*lettersCounter))
		return 0;

	printf("line 146\n");
	if(strncmp(linePointer, name, strlen(name) ) == 0 )
	{
		printf("line 148\n");
		printf("\nequal %d, %s", *lettersCounter, linePointer);
		return 1;
	}
	return 0;
}

void skipSpaceTab(char *p)
{
	while((*p) != '\n')
	{
		if((*p) == ' ' || (*p) == '\t')
		{
			memmove(p, p+1, strlen(p));
			continue;
		}
		else
			return;
	}
}

void removeSpace(char *linePointer)
{
	int i;
	int count=0;
	for(i=0 ; linePointer[i] ; i++)
	{
		if(linePointer[i] != ' ' && linePointer[i] != '\t' && linePointer[i] != '\n')
			linePointer[count++] = linePointer[i];
	}
	linePointer[count] = '\0';
}


char *create_new_macro_file(char *oldFileName, char *ending, int originalLength)
{
	/*we store the size of the old file name */
	int oldFileLength = strlen(oldFileName);
	/*if the size is less then the oldFile we need to delete extra chars from the name of file. */
	if(originalLength < oldFileLength)
	{
		oldFileName[oldFileLength - (oldFileLength - originalLength)] = '\0';
	}
	
	strcat(oldFileName, ending);
	return oldFileName;
}

ItemList* createList()
{
	ItemList *list = (ItemList*)malloc(sizeof(ItemList));
	list->head = NULL;
	return list;
}

void addToList(ItemList *list, char name[], char *pointToTable[], int indx_table)
{
	ItemMacro *p = (ItemMacro*)malloc(sizeof(ItemMacro));
	ItemMacro *prev, *curr;
	int i;

	strcpy(p->name, name);
	printf("\nline 226\n");
	printf("\np: %s, \n", pointToTable[1] );
	for(i=0; i<=indx_table ; i++){
		strcpy(p->macro_table[i], pointToTable[i]);
	}
	
	printf("\nline 232\n");
	prev = NULL;
	curr = list->head;

	while(curr != NULL )
	{
		prev = curr;
		curr = curr->next;
	}	

	p->next = curr;

	if(prev == NULL)
		list->head = p;
	else
		prev->next = p;
	list->macro_count += 1;
}


void printItem(ItemMacro *p){
	int i;
	printf("\nName: %s,\n", p->name);
	
	for(i=0; p->macro_table[i] ; i++){
		
			printf("Price: %s,\n", p->macro_table[i]);
	}
	
}

void printList(ItemList* list){
	ItemMacro *p;
	printf("\nHead---> ");
	for(p= list->head ; p != NULL ; p= p->next){
		printItem(p);
		printf("\n ---> \n");
	}
	printf(" NULL\n");
	
}
