#include "constant.h"
#include "inspectLine.h"
#include "data_structure.h"
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
void label_exist(char *label_name, int *error)
{
    symbol_table *p;
    

    for(p=symbol_head ; p ; p=p->next )
    {
        if(!strcmp(label_name, p->symbol))
        {
            printf("\nError: symbol already exist.");
            *error = 1;
            return;
        }
    }
}