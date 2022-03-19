/* 
    Two pass assembler for imaginary computer
    @author - Or Bacalu 
    @author - Ilay Katz
    @date - 19/03/2022
*/

#include "constant.h"
#include "data_structure.h"
#include "assembler.h"
#include "phaseOne.h"
#include "pre_assembler_phase.h"
#include "phaseTwo.h"
#include "manage_line.h"

int main(int argc, char *argv[])
{
    int i=1;
    char *file_name=NULL;

    if(argc < 2)
    {
        printf("\nError: No file arguments were enterd");
        exit(0);
    }

    for(i=1; i<argc ; i++)
    {
        file_name = argv[i];

        /*pre assembler phase - macro check */
	    macro(file_name);
        
        /*manage phase one and phase two */
        manage(file_name);
    }

    return 0;
}






FILE *file_open(char *file_name, char *file_ending, int amType)
{
    FILE *filePointer=NULL;
    char permission[3] = "r";
    char file_name_with_ending[MAX_FILE_NAME_LENGTH] = {'\0'};

    if (strcmp(file_ending, "am") == 0)
    {
        sprintf(file_name_with_ending, "%s.%s", file_name, "am"); /* build the file name for the fopen command*/
		if(amType)
        	strcpy(permission, "w+"); /*setting the permission for the file */
		else
			strcpy(permission, "r");
	
        filePointer = fopen(file_name_with_ending, permission);
    }

	else if(strcmp(file_ending, "as") == 0)
	{
		sprintf(file_name_with_ending, "%s.%s", file_name, "as"); 

        strcpy(permission, "r");

        filePointer = fopen(file_name_with_ending, permission);
	}

    else if (strcmp(file_ending, "ob") == 0)
    {
        sprintf(file_name_with_ending, "%s.%s", file_name, "ob"); 

        strcpy(permission, "w");

        filePointer = fopen(file_name_with_ending, permission);
    }

    else if (strcmp(file_ending, "ent") == 0)
    {
        sprintf(file_name_with_ending, "%s.%s", file_name, "ent"); 

        strcpy(permission, "w");

        filePointer = fopen(file_name_with_ending, permission);
    }

    else if (strcmp(file_ending, "ext") == 0)
    {
        sprintf(file_name_with_ending, "%s.%s", file_name, "ext"); 

        strcpy(permission, "w");

        filePointer = fopen(file_name_with_ending, permission);
    }

    return filePointer;
}






void manage(char* file_name)
{
    FILE *fp=NULL;
	head_of_data_lines *data_lines_list = NULL;
	head_of_symbol_list *symbol_list = NULL;
	char file_name_remove[MAX_FILE_NAME_LENGTH]= {'\0'}; /*temporary file name so we can remove the files if an error occurred */
	int error=0; /*flag to know if there was an error in the program */
    int entry_flag=0, external_flag=0; /*flags to know if there is entry or external, to create .ent and .ext files */
    essentials *menu = create_essentials();


    symbol_list = create_symbol_list();

	data_lines_list = create_data_list();

    fp = file_open(file_name, "am",READ_ONLY);
	
    manage_phaseOne(menu,fp, data_lines_list, symbol_list, &error, &external_flag, &entry_flag);
    
	fp=NULL;
    
    fp = file_open(file_name, "am",READ_ONLY);

	manage_phaseTwo(menu,fp,file_name, data_lines_list, symbol_list, &error );

	
	if(error)   /*if there was an error in the file then we remove the output files. */
    {
		sprintf(file_name_remove, "%s.%s", file_name, "ent"); /* builds the filename for the fopen */
		remove(file_name_remove);
       	sprintf(file_name_remove, "%s.%s", file_name, "ob"); /* builds the filename for the fopen */
		remove(file_name_remove);
		sprintf(file_name_remove, "%s.%s", file_name, "ext"); /* builds the filename for the fopen */
		remove(file_name_remove);
        sprintf(file_name_remove, "%s.%s", file_name, "am"); /* builds the filename for the fopen */
		remove(file_name_remove);
    }

    if(!entry_flag)
    {
        sprintf(file_name_remove, "%s.%s", file_name, "ent"); /* builds the filename for the fopen */
		remove(file_name_remove);
    }

    if(!external_flag)
    {
        sprintf(file_name_remove, "%s.%s", file_name, "ext"); /* builds the filename for the fopen */
		remove(file_name_remove);
    }

	/*free all the memory in symbol and data list that have been allocated */
	free_symbol_table_memory(symbol_list);
	free_data_line_memory(data_lines_list);
}

