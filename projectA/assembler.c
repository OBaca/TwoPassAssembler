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
        
        file_name = (char*)malloc(sizeof(strlen(argv[i])+4));
        strcpy(file_name, argv[i]);

        argv_length = strlen(file_name);
	    macro(file_name, argv_length);
        
        manage(file_name, argv_length);
        
        free(file_name);
    }


    
    return 0;
}



FILE* get_file(char *file,int argv_length, char *ending, char *permission)
{
    FILE *fp;
    int current_file_length = strlen(file);
    
    /*open file named .am and point to it. */
    if(!(fp= fopen(create_new_file(file, ending, current_file_length, argv_length), permission))){ 
		printf("\nError: cannot open file name: %s.am\n",file); 
		exit(0);
	}
        

    return fp;

}

char *create_new_file(char *oldFileName, char *ending, int current_file_length, int argv_length)
{
	
    
	/*if the size is less then the oldFile we need to delete extra chars from the name of file. */
	if(current_file_length > argv_length)
	{
		oldFileName[current_file_length - (current_file_length - argv_length)] = '\0';
	}

	strcat(oldFileName, ending);


    
	return oldFileName;
}

void manage(char* file_name, int argv_length)
{
    FILE *fp;
	head_of_data_lines *data_lines_list = NULL;
	head_of_symbol_list *symbol_list = NULL;
	int error=0; /*flag to know if there was an error in the program */

    symbol_list = create_symbol_head();

	data_lines_list = create_data_list();

    fp = get_file(file_name, argv_length, ".am", "r");
	

    manage_phaseOne(fp, data_lines_list, symbol_list, &error);
    
    

	fp=NULL;
    
    fp = get_file(file_name, argv_length, ".am", "r");
	manage_phaseTwo(fp,file_name, data_lines_list, symbol_list, &error, argv_length );


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

