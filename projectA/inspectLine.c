#include "constant.h"
#include "inspectLine.h"
#include "macro.h"
#include "data_structure.h"
#include "manage_line2.h"
#include "errors.h"

/*initialize data counter(DC), and instrcution counter(IC) */
int DC = 0;
int IC = 100;

symbol_table *symbol_head = NULL;

enum {DATA, CODE, ENTRY, EXTERNAL};
enum {SOURCE, DEST};
enum {A=16, R, E};

int main(int argc, char *argv[])
{
    
    manage(argv[1]);
    
    return 0;
}

void manage(char* file)
{
    FILE *fp;

    fp = get_file(file);
   

    manage_line(fp);
}

FILE* get_file(char *file)
{
    FILE *fp;
    int nameLength = strlen(file);

    /*open file named .am and point to it. */
    if(!(fp= fopen(create_new_macro_file(file, ".am",nameLength ), "r"))){ 
		printf("\nError: cannot open file name: %s.am\n",file); 
		exit(0);
	}
    
    return fp;

}


void manage_line(FILE *fp)
{
    char *linePointer;
    char line[LENGTH_LINE];
    int error=0; /*flag to know if there was an error in the program */
    int label_flag = 0; /*to indicate if there is a label */
    char *label_name;
    int line_counter=0;
    int lettersCounter=0;
    int symbol_type;


    while(fgets(line, LENGTH_LINE, fp) != NULL)
    {
        line_counter++;
        linePointer = line;
       

        if(*linePointer == EOF){
            if(error)
                exit(0);

            /*algorithem move to 17. */
                
        }

       

        /*skipping all the first spaces and tabs */
        skipSpaceTab(linePointer);

        /*check if we got a comment */
        COMMENT_CHECK

        /*if there is a label we turn the label_flag on and save his name */
        label_name = is_label(linePointer, &label_flag, &error, line_counter) ;
        
        if(label_name != NULL)
            linePointer += strlen(label_name) +1;
            
         
        if(label_flag)
            skipSpaceTab(linePointer);

        
        lettersCounter = 0; /*resetting the counter */



        /*WE NEED TO CHANGE THE FUNCTION ( LIKE MAMAN 22 ) */
        if(word_check(linePointer, ".data", &lettersCounter))
        {
            linePointer += lettersCounter; /*updating the pointer */
            symbol_type = DATA;

            if(label_flag)
            {
                label_exist(label_name  ,&error);
                
                add_label(label_name, symbol_type);
            }

            reading_data_line(linePointer);
            continue;
        }

        

        
                
    }


}



void remove_end_white_chars(char *linePointer)
{
    int last_char=0;
    int i;
    for(i=0 ; linePointer[i] ; i++)
    {
        if(linePointer[i] != ' ' && linePointer[i] != '\t' && linePointer[i] != '\n')
            last_char = i;
    }

    linePointer[last_char +1] = '\0';
}


char *is_label(char *linePointer, int *label_flag, int *error, int line_counter)
{
    char *name = save_name(linePointer); /*saving label name */
    int length_name = strlen(name);

    /*it means there is a label */
    if(name[length_name-1] == ':')
    {
        if(valid_label(name, &error, line_counter))
        {
            *label_flag = 1;
            name[length_name - 1] = '\0';
            return name;
        }

    }

    return NULL;
}

void add_label(char *label_name, int symbol_type)
{
    symbol_table *p = (symbol_table*)malloc(sizeof(symbol_table));
     
	macro_table* temp;
	/*copying the information to the macro node */
    strcpy (p->symbol , label_name);
    p->next = NULL;
    switch(symbol_type)
    {
        case DATA:
            strcpy(p->attributes, "data");
            break;
        case CODE:
            strcpy(p->attributes, "code");
            break;
        case ENTRY:
            strcpy(p->attributes, ", entry");
            break;
        case EXTERNAL:
            strcpy(p->attributes, "external");
            break;
        default: 
            break;
    }
    p->value = IC;
    p->offset = IC % 16;
	p->base_address = IC - p->offset;

	/*adding the node to the linked list */
    	if(symbol_head == NULL)
    	{
        	symbol_head = p;
        	return;
    	}
    	else
    	{
        	temp = symbol_head;
        	while(temp->next != NULL)
            	temp = temp->next;
        	temp->next = p;
    	}
}


/*
void reading_data_line(char *linePointer)
{
    if(*linePointer != ' ')

}

*/


/*WE HAVE SIMILAR FUNCTION IN MACRO.C (NAME_CHECK) NEED TO THINK ON SOMETHING IN COMMON ! ! ! ! ! ~~~ ! ~ ! ~ ! */
/*!#@&!##@$(!*#&(*@!$&*(!#&(!#&(#&&!($())))))) */

int word_check(char *linePointer, char *name, int *lettersCounter)
{
	int j=0;
	
	/*count the chars of the word that the pointer points to*/
	while(linePointer[j] && linePointer[j] != ' ' && linePointer[j] != ',' && linePointer[j] != '\t' && linePointer[j] != '\n' )
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
