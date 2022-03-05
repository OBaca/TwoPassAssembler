#include "constant.h"
#include "macro.h"
#include "data_structure.h"
#include "assembler.h"

head_of_list *list = NULL;

/*This function gets a FILE POINTER ! , DONT FORGET TO CHANGE $$ */
void macro(char *file, int argv_length)
{
	FILE *fp=NULL, *newfp=NULL; /*fp pointer to .as, newfp pointer to .am */
	char line[LENGTH_LINE]; /*to store each line in the file */
	char line_copy[LENGTH_LINE]; /*a copy of the original line */
	char *linePointer; 
	int lettersCounter=0; 
	char temp_macro_table[LENGTH_MACRO][LENGTH_LINE]; /*macro table - to store macro name and the lines for the macro */
	int num_of_macro_lines=0; /*number of lines in a macro*/
	int macro_signal=0; /*flag to know if we are inside a macro */
    char macro_name1[LENGTH_LINE]; /*to store the macro name*/
	int skip_macro_name; /*flag to know if we skip the macro name */
	char *point_to_label_name;
	create_list(); /*creates a linked list */

	/*open file named .as and point to it. */
	fp = get_file(file,argv_length, ".as", "r");
	/*create new file named .am */
	newfp = get_file(file, argv_length, ".am", "w+");


	/*reading through the lines in the file */
	while(fgets(line,LENGTH_LINE , fp) != NULL)	
	{
		strcpy(line_copy, line);
		
		skip_macro_name = 0;
		/*check if one of the macros names was called, and insert the lines */
		transfer_macro(line_copy, newfp, &skip_macro_name);
		
		linePointer = line;
		/*if the function transfer_macro completed, we skip to the next line */
		if(!skip_macro_name)
		{
			if(!macro_signal) /*if we are not in a macro*/
			{
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
				if(*linePointer != ' ' && *linePointer != '\t')
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
				point_to_label_name = save_name(linePointer);

				if(point_to_label_name != NULL){
					strcpy(macro_name1,point_to_label_name);
					free(point_to_label_name);
				}
				/*we are now inside a macro*/
				macro_signal = 1;          		
			}
			/*if we are insied a macro*/
			else
			{
				/*skipping spaces and tabs */
				skipSpaceTab(linePointer);
				/*reset the index*/
				lettersCounter = 0;
				
				/*check for end of the macro*/
				if(!( name_check(linePointer, "endm", &lettersCounter) )) 
				{
					/*not the end, save line to the macro table*/
					strcpy(temp_macro_table[num_of_macro_lines], line);
					num_of_macro_lines++;
					continue;
				}

				/*create node with the macro information*/
				add_node( macro_name1, temp_macro_table,num_of_macro_lines);

				/*reset the flags*/
				num_of_macro_lines=0;
				lettersCounter=0;
				macro_signal = 0;
			}
		}		
		
	}

	free_memory(); /*free all the memory that have beem allocated */  
	fclose(fp);
	fclose(newfp); 
}


char * save_name(char *linePointer)
{
	int j;
	int name_length = 0;
	char *name=NULL;

	/*count the length of the current word in line*/
	for(j=0 ; linePointer[j] && linePointer[j] != '\n' && linePointer[j] != ' ' && linePointer[j] != '\t'; j++ )
	{
		name_length++;
	}

	name = (char *)malloc(sizeof(char) * (name_length+1));
	if(name == NULL)
	{
		printf("\nError: Memory allocation failed.");
                exit(0);
	}

	/*copy the word to a "name" string*/
	for(j=0; j < name_length ; j++)
	{
		name[j] = linePointer[j];
	}
	name[j] = '\0';
	return name;
}



int name_check(char *linePointer, char *name, int *lettersCounter)
{
	int j=0;
	*lettersCounter =0;
	/*count the chars of the word that the pointer points to*/
	while((linePointer[j] >= 'A' && linePointer[j] <= 'Z') || (linePointer[j] >= 'a' && linePointer[j] <= 'z') || (linePointer[j] >= 0 && linePointer[j] <= 9) || linePointer[j] == '#' || linePointer[j] == '.' )
	{
		(*lettersCounter)++;
		j++;
	}
	/*checking if the word is bigger then it should be */
	if(strlen(name) < (*lettersCounter))
		return 0;
	
	if(strncmp(linePointer, name, strlen(name) ) == 0 )
	{
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
	linePointer[count] = '\0'; /*to finish the string*/
}





void create_list()
{
	list = (head_of_list*)malloc(sizeof(head_of_list));
	if(list == NULL)
     {
        printf("\nError: Memory allocation failed.");
		exit(0);
     }
	list->head = NULL;
}

void add_node(char* name, char temp_macro_table[LENGTH_MACRO][LENGTH_LINE], int num_of_macro_lines)
{
	int j;
  	macro_table* p = (macro_table*)malloc(sizeof(macro_table));
	macro_table* temp;
	if(p == NULL)
	{
		printf("\nError: Memory allocation failed.");
		exit(0);
	}
	
	/*copying the information to the macro node */
    strcpy (p->macro_name , name);
    for(j = 0; j < num_of_macro_lines; j++)
    {
    	strcpy(p->macro_lines[j] , temp_macro_table[j]);
    }
		
	p->line_max_indx = num_of_macro_lines;
    p->next = NULL;
	
	/*adding the node to the linked list */
    if(list->head == NULL)
    {
    	list->head = p;
    	return;
    }
	else
    {
    	temp = list->head;
     	while(temp->next != NULL)
           	temp = temp->next;
    	temp->next = p;
	}	
}



void transfer_macro(char line[],  FILE *newfp, int *skip_macro_name)
{
	int lettersCounter=0;
 	int i;
	macro_table* curr = list->head;
	
	removeSpace(line); /*removing all the white tabs in the line */

	while( curr != NULL )
    {
	/*checking if the word in the line is one of the macros we have saved */
    	if(name_check(curr->macro_name, line, &lettersCounter) )
    	{
			*skip_macro_name = 1; /*turning the flag on */
			/*we transfer the macro lines to the new file */
        	for(i=0 ; i < curr->line_max_indx ; i++){
            	fputs(curr->macro_lines[i], newfp);
			}
        }
			
        curr = curr->next;
    	}
}



void free_memory()
{
	macro_table* temp;
	while(list->head != NULL)
	{
		temp = list->head ;
		list->head  = list->head -> next;
		free(temp);	
	}
	free(list);
}



