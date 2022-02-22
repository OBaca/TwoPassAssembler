#include "constant.h"
#include "data_structure.h"
#include "inspectLine.h"
#include "errors.h"
#include "macro.h"
#include "manage_line2.h"


/*initialize data counter(DC), and instrcution counter(IC) */
int DC = 0;
int IC = 100;


/*data image array */
 line *data_image[LENGTH_DATA];

enum {DATA, CODE, ENTRY, EXTERNAL};
enum {SOURCE, DEST};
enum {A=4, R=2, E=1};

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
    char label_name[LENGTH_LABEL];
	char *point_to_label_name;
    int line_counter=0;
    int lettersCounter=0;
    int symbol_type;
	head_of_symbol_list *symbol_list = NULL;
    symbol_list = create_symbol_head();

    
    while(fgets(line, LENGTH_LINE, fp) != NULL)
    {
        label_flag=0;
        line_counter++;
        linePointer = line;

       printf("\n START LOOP\n");
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
        point_to_label_name = is_label(linePointer, &label_flag, &error, line_counter);

		printf("\nafter is label line 97: %s", linePointer);
        if(point_to_label_name !=NULL)
        {
            strcpy(label_name, point_to_label_name );
            free(point_to_label_name);
			linePointer += strlen(label_name) +1;
        }
        
            
            
        printf("\nbefore label check line 103: %s", linePointer);

        if(label_flag)
            skipSpaceTab(linePointer);

		printf("\nbefore word check line 107: %s", linePointer);
        if(word_check(linePointer, ".data", &lettersCounter) || word_check(linePointer, ".string", &lettersCounter))
        {
            
            symbol_type = DATA;

			printf("\nbefore label flag check --> %s", linePointer);
            if(label_flag)
            {
                if(symbol_list != NULL && symbol_list->head != NULL)
                    label_exist(symbol_list ,label_name  ,&error);

                add_label(symbol_list ,label_name, symbol_type);
                
            }
			printf("\n%s<---after label flag check\n", linePointer);
            
            if(word_check(linePointer, ".data", &lettersCounter) ){
                linePointer += lettersCounter; /*updating the pointer */
                reading_data_line(linePointer, &error, line_counter);
            }
            else{
                linePointer += lettersCounter; /*updating the pointer */
                reading_string_line(linePointer, &error, line_counter);
            }
            continue;
        }

			
            
        

        	

                
    }
	/*!~!~!~!~~!~!~ TEMPORARAY FOR DEBUGGING!!@@!@!@~!~!~@~!~@~ */
		print_all_symbols(symbol_list);
			print_data_table();

		/*free all the left memory in symbol_list */
	    free_symbol_table_memory(symbol_list);

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
    char *name;
    int length_name;
    name = save_name(linePointer); /*saving label name */
    length_name = strlen(name);

    /*if there is ':' in the end of the name its a label */
    if(name[length_name-1] == ':')
    {
        if(valid_label(name, error, line_counter))
        {
            *label_flag = 1;
            name[length_name - 1] = '\0';
            return name;
            
        }

    }
    free(name);
    return NULL;
}

void add_label(head_of_symbol_list* symbol_list ,char *label_name, int symbol_type)
{
    symbol_table *p = (symbol_table*)malloc(sizeof(symbol_table));
	symbol_table* temp;

    if(p==NULL)
    {
        printf("Error: Memory allocation failed.");
		exit(0);
    }
	/*copying the information to the symbol node */
    strcpy (p->symbol , label_name);

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
    p->next = NULL;
    

	/*adding the node to the linked symbol_list */
    	if(symbol_list->head == NULL)
    	{
        	symbol_list->head = p;
        	return;
    	}
    	else
    	{
        	temp = symbol_list->head;
        	while(temp->next != NULL)
            	temp = temp->next;
        	temp->next = p;
    	}
}



/*WE HAVE SIMILAR FUNCTION IN MACRO.C (NAME_CHECK) NEED TO THINK ON SOMETHING IN COMMON ! ! ! ! ! ~~~ ! ~ ! ~ ! */
/*!#@&!##@$(!*#&(*@!$&*(!#&(!#&(#&&!($())))))) */
int word_check(char *linePointer, char *name, int *lettersCounter)
{
	int j=0;
	*lettersCounter = 0;
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



void reading_data_line(char *linePointer, int *error, int line_counter)
{
	int lettersCounter = 0;
	int x;
	int errorInInt=0;
	int commaFlag =0;
	int is_Empty = 1;

    /*we need to have space between the command to the parameters
        check if we got undefined directive command, and prints an error if needed */
    UNDEFINED_DIR_CMD

    /*we have space so we need to skip till the parameters */
    skipSpaceTab(linePointer);

    /*check if we got an illegal comma between the command to the parameters, and prints an error if needed */
    if(*linePointer == ',')
    {
        ILLEGAL_COMMA
    }

    while(*linePointer && (*linePointer) != '\n' )
    {
		commaFlag=0;

        /*checking for an illegal parameter, and prints an error if needed */
        if( (*linePointer < '0' || *linePointer > '9') && *linePointer != '-')
        {
            ILLEGAL_PARAM
        }

		/*The parameter now is valid so we save it to the data_image */
        x = intCheck(linePointer, &errorInInt, &lettersCounter);
		
		if(errorInInt)
		{
			ILLEGAL_PARAM
		}

		linePointer += lettersCounter;

		skipSpaceTab(linePointer);

		if(*linePointer == ',')
		{
			commaFlag=1;
			linePointer++;
		}

		skipSpaceTab(linePointer);

        /*prints an error if there is multiple commas. */
		if(*linePointer == ',' && commaFlag)
		{
			MULTIPLE_COMMAS
		}

		if((*linePointer == '\n' || !(*linePointer) ))
		{
			if(*(linePointer-1) < '0' || *(linePointer-1) > '9')
			{	
				*error = 1;
				return;
			}
		}
		printf("\nin line 332 = x: %d\n", x);
		data_image[IC] = add_data_parameter(x);
		is_Empty=0;
		IC++;
		DC++;

    }

	if(is_Empty)
	{
		printf("\nError: You didn't insert parameters after \'.data\' directive. In line: %d",line_counter);
		*error =1;
		return;
	}
}



int intCheck(char *linePointer, int *errorInInt, int *lettersCounter)
{
	char *numberChar; /*char array to save the number */
	int numberLength; /*the length of the number included the dot */
	int flagMinus=0; /*flag if the number is negative */
	int x;

	/*resetting the counters */
	*errorInInt = 0; 
	*lettersCounter = 0;

	if((*linePointer) == '-') 
	{
		flagMinus =1;
		linePointer++;
	}

	numberLength = countNum(linePointer);
	
	/*if the length of the number is less then 1 we have an invalid parameter */
	if(numberLength <= 0)
	{
		*errorInInt = 1;
		return 0; /*Doesn't matter what we will return if the flag value is 1 */
	}

	/*making a char array for the number */
	numberChar = createIntChar(linePointer, numberLength);
	
	/*if there is minus we need to update the counter to the correct length */
	numberLength += (flagMinus) ? 1:0; 

	(*lettersCounter) = numberLength; /* updating the current index */
	
	x = atoi(numberChar); /*convert the number to a int*/
	free(numberChar); /*prevent memory leak */
	
	if(flagMinus)  /*if the flag is 1 we will make the number negative */
		return (-1) * x;
	return x;
}


char *createIntChar(char *linePointer, int numberLength)
{
	/*Making a char array for the number */
	char *str = malloc(numberLength +1);
	char *q;
	int j =0;
	
	q = linePointer;
	
	while(j<numberLength)
	{
		str[j] = (*q);
		j++;
		q++;
	}
	str[j] = '\0';
	return str;
}

int countNum(char *linePointer)
{
	int counter =0; /*counter to count the length of the number */
	int i;
	
	for(i=0 ; linePointer[i] && linePointer[i] != ' ' && linePointer[i] != '\t' && linePointer[i] != '\n'; i++)
	{
		
		if(isdigit((*(linePointer+i)))) 
		{
			counter++;
			continue;
		}
		
		if((*(linePointer+i)) == ',') /*we reached end of number */
		{
			return counter;
		}
		/*if its not a number or a comma, invalid parameter. */
		return -1;
	}
	return counter;
}

line *add_data_parameter(int x)
{
	line *p = (line*)malloc(sizeof(line));

	p->classify = A;
	p->signal = x;

	return p;
}



void reading_string_line(char *linePointer, int *error, int line_counter)
{
    

	/*we need to have space between the command to the parameters
        check if we got undefined directive command, and prints an error if needed */
    UNDEFINED_DIR_CMD

	/*we have space so we need to skip till the parameters */
    skipSpaceTab(linePointer);

	if(*linePointer != '"')
	{
		printf("\nError: Illegal string, In line: %d", line_counter);
		*error = 1;
		return;
	}

	linePointer++;

	while( *linePointer != '"'){
		if(!string_is_valid(linePointer, error, line_counter))
			return;

		data_image[IC] = add_data_parameter(*linePointer);
		
		IC++;
		DC++;

		linePointer++;
	}

	data_image[IC] = add_data_parameter(0);
	IC++;
	DC++;

	linePointer++;
	skipSpaceTab(linePointer);
	if(*linePointer != '\n' && *linePointer != '\0')
	{
		printf("\nError: Extraneous text, In line: %d",line_counter);
		*error = 1;
		return;
	}
}




/*~!~!~!~!~ for DEBUGGING please delete it LATER ~!~!~!~!~!~!~~!~!~!~!~!~!~ ~!~!~!~!~!~!~ ~!~!~!~!~!~!~ ~!~!~!~!~!~!~  */
void print_all_symbols(head_of_symbol_list* list)
{
    symbol_table *p;
    
    p = list->head;
    while(p!= NULL)
    {
        printf("\nSymbolName: %s\nSymbol_base: %d\nSymbol_offset: %d\nSymbol_value: %d\n", p->symbol, p->base_address, p->offset,p->value);
        p=p->next;
    }
}

void print_data_table()
{
	int i;
	for(i=100; i<IC; i++)
	{
		printf("\nData Line: %u\n",data_image[i]->signal);
	}
}
/* !~!~!~!~!~!~~!~!~!~!~!~!~ ~!~!~!~!~!~!~ ~!~!~!~!~!~!~ ~!~!~!~!~!~!~ !~!~!~!~!~!~~!~!~!~!~!~!~ ~!~!~!~!~!~!~ ~!~!~!~!~!~!~ ~!~!~!~!~!~!~ */


head_of_symbol_list *create_symbol_head()
 {
     head_of_symbol_list *list = (head_of_symbol_list*)malloc(sizeof(head_of_symbol_list));
     if(list == NULL)
     {
        printf("Error: Memory allocation failed.");
		exit(0);
     }
     list->head = NULL;
     return list;
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

