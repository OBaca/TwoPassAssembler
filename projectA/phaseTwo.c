#include "constant.h"
#include "data_structure.h"
#include "phaseOne.h"
#include "errors.h"
#include "macro.h"
#include "manage_line2.h"
#include "assembler.h"
#include "phaseTwo.h"




void manage_phaseTwo(FILE *fp,char *file_name, head_of_data_lines* data_lines_list, head_of_symbol_list *symbol_list, int *error, int argv_length)
{
    char line[LENGTH_LINE];
    char *linePointer=NULL;
    int line_counter=0;
    char *point_to_label_name = NULL;
    int label_flag=0;
    char label_name[LENGTH_LABEL];
    int lettersCounter;
    FILE *entFile; 
    entFile = get_file(file_name, argv_length, ".ent", "w+"); 

    while(fgets(line, LENGTH_LINE, fp) != NULL)
    {
        linePointer = line;
        line_counter++;
        lettersCounter=0;

       

        /*skipping all the first spaces and tabs */
        skipSpaceTab(linePointer);

        /*if its an empty line we skip */
        if(*linePointer == '\n' || *linePointer == '\0')
			continue;

        /*check if we got a comment */
        COMMENT_CHECK

        
        point_to_label_name = is_label(linePointer, &label_flag, error, line_counter);
        
        if(point_to_label_name !=NULL)
        {
            
            strcpy(label_name, point_to_label_name );
            free(point_to_label_name);
			linePointer += strlen(label_name) +1;
        }
        
        skipSpaceTab(linePointer);

        if(word_check(linePointer, ".data", &lettersCounter) || word_check(linePointer, ".string", &lettersCounter) || word_check(linePointer, ".extern", &lettersCounter))
            continue;

        
        /*if there is an .entry directive command we will check if its valid and add the entries lines to the .ent file */
        if(word_check(linePointer, ".entry", &lettersCounter) )
        {
            linePointer += lettersCounter;

            UNDEFINED_DIR_CMD
        
            skipSpaceTab(linePointer);

            if(*linePointer == '\n' || *linePointer == '\0')
            {
                printf("\nError: No label enterd after an .entry command. In line: %d", line_counter); 
                *error = 1; 
                continue;
            }
            
            point_to_label_name=NULL;

            point_to_label_name = save_name(linePointer);
            
            if(point_to_label_name !=NULL)
            {
                strcpy(label_name, point_to_label_name );
                free(point_to_label_name);
                create_entry_file(entFile,file_name, symbol_list, label_name, error, line_counter);
                linePointer += strlen(label_name) +1;
            }

            skipSpaceTab(linePointer);

            EXTRANEOUS_TEXT
            
            continue;
        } /*end of .entry check */
        
        


    }
    
    if(*error)
    {
        remove(file_name);
    }
    fclose(fp);


}

void create_entry_file(FILE *entFile, char *file_name, head_of_symbol_list* symbol_list ,char *label_name, int *error, int line_counter)
{
    symbol_table *temp = symbol_list->head;
    
    char entry_values[9];
    
    
    while(temp != NULL)
    {
       
        if(!strcmp(label_name, temp->symbol) )
        {
            fputs(temp->symbol, entFile);
            
            sprintf(entry_values, ",%i,%i\n", temp->base_address, temp->offset);

            fputs(entry_values,entFile);
            
            
            return;

        }

        temp = temp->next;
    }

    
    printf("\nError: Label doesn't exist. in line: %d", line_counter);
    *error = 1;
    
    

}
