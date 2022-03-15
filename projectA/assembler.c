#include "constant.h"
#include "data_structure.h"
#include "assembler.h"
#include "phaseOne.h"
#include "macro.h"
#include "phaseTwo.h"
#include "manage_line2.h"

int main(int argc, char *argv[])
{
    int argv_length;
    int i;
    char *file_name;

    for(i=1; i<argc ; i++)
    {
        
        file_name = argv[i];
	
        argv_length = strlen(file_name);

	macro(file_name, argv_length);
        
        manage(file_name, argv_length);
       
    }


    
    return 0;
}



FILE *file_open(char *fileName, char *fileType, int amType)
{
    FILE *filePointer;
    char fileState[3] = "r";
    char workingfile[255];

    if (strcmp(fileType, "am") == 0)
    {
        sprintf(workingfile, "%s.%s", fileName, "am"); /* builds the filename for the fopen */
		if(amType)
        	strcpy(fileState, "w+");
		else
			strcpy(fileState, "r");
	
        filePointer = fopen(workingfile, fileState);
    }

	else if(strcmp(fileType, "as") == 0)
	{
		sprintf(workingfile, "%s.%s", fileName, "as"); /* builds the filename for the fopen */

        strcpy(fileState, "r");

        filePointer = fopen(workingfile, fileState);
	}

    else if (strcmp(fileType, "ob") == 0)
    {
        sprintf(workingfile, "%s.%s", fileName, "ob"); /* builds the filename for the fopen */

        strcpy(fileState, "w");

        filePointer = fopen(workingfile, fileState);
    }

    else if (strcmp(fileType, "ent") == 0)
    {
        sprintf(workingfile, "%s.%s", fileName, "ent"); /* builds the filename for the fopen */

        strcpy(fileState, "w");

        filePointer = fopen(workingfile, fileState);
    }

    else if (strcmp(fileType, "ext") == 0)
    {
        sprintf(workingfile, "%s.%s", fileName, "ext"); /* builds the filename for the fopen */

        strcpy(fileState, "w");

        filePointer = fopen(workingfile, fileState);
    }

    return filePointer;
}

void manage(char* file_name, int argv_length)
{
    FILE *fp;
	head_of_data_lines *data_lines_list = NULL;
	head_of_symbol_list *symbol_list = NULL;
	char file_name_remove[255];
	int error=0; /*flag to know if there was an error in the program */

    symbol_list = create_symbol_head();

	data_lines_list = create_data_list();

    fp = file_open(file_name, "am",0);
	

    manage_phaseOne(fp, data_lines_list, symbol_list, &error);
    
    

	fp=NULL;
    
    fp = file_open(file_name, "am",0);
	manage_phaseTwo(fp,file_name, data_lines_list, symbol_list, &error, argv_length );

	
	if(error)
    {
		sprintf(file_name_remove, "%s.%s", file_name, "ent"); /* builds the filename for the fopen */
		remove(file_name_remove);
       	sprintf(file_name_remove, "%s.%s", file_name, "ob"); /* builds the filename for the fopen */
		remove(file_name_remove);
		sprintf(file_name_remove, "%s.%s", file_name, "ext"); /* builds the filename for the fopen */
		remove(file_name_remove);
        
        
        /* remove AM check MAYBE */
    }
	/*free all the memory in symbol and data list */
	free_symbol_table_memory(symbol_list);
	free_data_line_memory(data_lines_list);
}



void free_symbol_table_memory(head_of_symbol_list* list)
{
    symbol_table* p;
    symbol_table* temp;

    if(list == NULL)
        return;
    p = list->head;
    while(p != NULL)
    {
        temp = p;
        p = p->next;
        free(temp);
    }
    free(list);
}

void free_data_line_memory(head_of_data_lines* list)
{
    data_lines* p;
    data_lines* temp;

    if(list == NULL)
        return;
    p = list->head;
    while(p != NULL)
    {
        temp = p;
        p = p->next;
        free(temp);
    }
    free(list);
}

