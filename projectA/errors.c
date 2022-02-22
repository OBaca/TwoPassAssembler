#include "constant.h"
#include "data_structure.h"
#include "inspectLine.h"
#include "errors.h"




int valid_label(char *name, int *error, int line_counter)
{
    int i=0;
    int name_length = strlen(name);
    int not_valid_flag = 0;

    /*checking if the length of the label is valid */
    if(name_length > LENGTH_LABEL)
        not_valid_flag =1;

    /*checking if the first letter is not a letter */
    if( (name[i] < 'a' || name[i] > 'z') && (name[i] < 'A' || name[i] > 'Z' ))
        not_valid_flag =1;
    
    /*checking if the label name is valid (only letters and numbers) */
    for(i =1; i < name_length-1 ; i++)
        if((name[i] < 'a' || name[i] > 'z') && (name[i] < 'A' || name[i] > 'Z' ) && (name[i] < '0' || name[i] > '9'))
            not_valid_flag =1;
    
    /*if the label is not valid, print an error. */
    if(not_valid_flag)
    {
        printf("Error: not a valid label in line: %d.", line_counter);
        *error = 1;
        return 0;
    }

    /*the label is valid */
    return 1;
}

/*label_exist, checks if there is already a symbol with the same name in our table */
void label_exist(head_of_symbol_list* list ,char *label_name, int *error)
{
    symbol_table *temp = list->head;

    while(temp->next != NULL)
    {
        if(!strcmp(label_name, temp->symbol))
        {
            printf("\nError: symbol already exist.");
            *error = 1;
            return;
        }
        temp = temp->next;
    }
}


int string_is_valid(char *linePointer, int *error, int line_counter){
    if( (*linePointer < 32 && *linePointer != 9) ||  *linePointer > 126 )
            {
                if(*linePointer == '\n' || *linePointer == '\0')
                    printf("Error: Missing closing quotation marks in a string command, In line: %d", line_counter);
                else
                    printf("\nError: Not a valid ascii char, In line: %d", line_counter);
                
                *error=1;
                return 0;
            }
    return 1;
} 

