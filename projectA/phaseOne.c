#include "constant.h"
#include "data_structure.h"
#include "phaseOne.h"
#include "errors.h"
#include "macro.h"
#include "manage_line2.h"
#include "assembler.h"


/*initialize data counter(DC), and instrcution counter(IC) */
int DC = 0;
int TIC = 100; /*temp IC */




enum {DATA, CODE, ENTRY, EXTERNAL};
enum {SOURCE, DEST};
enum {A=4, R=2, E=1};



void manage_phaseOne(FILE *fp, head_of_data_lines* data_lines_list, head_of_symbol_list *symbol_list, int *error)
{
    char *linePointer=NULL;
    char line[LENGTH_LINE];
    
    int label_flag = 0; /*to indicate if there is a label */
    char label_name[LENGTH_LABEL];
	char *point_to_label_name=NULL;
    int line_counter=0;
    int lettersCounter=0;
    int symbol_type=0;
	int data_flag = 0;

    

    
    while(fgets(line, LENGTH_LINE, fp) != NULL)
    {
        label_flag=0;
		data_flag = 0;
        line_counter++;
        linePointer = line;
		point_to_label_name=NULL;

       
        if(*linePointer == EOF){
            if(*error)
                exit(0);

            /*algorithem move to 17. */


        }
		

        /*skipping all the first spaces and tabs */
        skipSpaceTab(linePointer);
	
		/*if its an empty line we skip */
		if(*linePointer == '\n' || *linePointer == '\0')
			continue;

        /*check if we got a comment */
        COMMENT_CHECK

		

        /*if there is a label we turn the label_flag on and save his name */
        point_to_label_name = is_label(linePointer, &label_flag, error, line_counter);

		
		
        if(point_to_label_name !=NULL)
        {
            strcpy(label_name, point_to_label_name );
            free(point_to_label_name);
			linePointer += strlen(label_name) +1;
        }
        
            
            
		

        if(label_flag)
            skipSpaceTab(linePointer);

		if(*linePointer == '.')
			data_flag = 1;
		
        if(word_check(linePointer, ".data", &lettersCounter) || word_check(linePointer, ".string", &lettersCounter))
        {
            
            symbol_type = DATA;

			
            if(label_flag)
            {
                if(symbol_list != NULL && symbol_list->head != NULL)
                    label_exist(symbol_list ,label_name  ,error, symbol_type);

                add_label(symbol_list ,label_name, symbol_type);
                
            }
			
            
            if(word_check(linePointer, ".data", &lettersCounter) ){
				
                linePointer += lettersCounter; /*updating the pointer */
                reading_data_line(data_lines_list ,linePointer, error, line_counter);
            }
            else{
				
                linePointer += lettersCounter; /*updating the pointer */
                reading_string_line(data_lines_list,linePointer, error, line_counter);
            }
						

            continue;
        }

					
		
		if(word_check(linePointer, ".entry", &lettersCounter) )
			continue;
		
	
        if(word_check(linePointer, ".extern", &lettersCounter))
        {
            linePointer += lettersCounter;
            symbol_type = EXTERNAL;

			add_extern(symbol_list, linePointer, symbol_type, error, line_counter);
            
            continue;
        }

		if(data_flag)
		{
			printf("\nError: Undefined directive command. In line: %d", line_counter); 
            *error = 1; 
            continue; 
		}


		if(label_flag)
        {
			symbol_type = CODE;

            if(symbol_list != NULL && symbol_list->head != NULL)
                label_exist(symbol_list ,label_name  ,error, symbol_type);

            add_label(symbol_list ,label_name, symbol_type);
            
        } 
			
		skipSpaceTab(linePointer);
		
		manage_code_lines(linePointer);
		
                
    }

	fclose(fp);
	/*!~!~!~!~~!~!~ TEMPORARAY FOR DEBUGGING!!@@!@!@~!~!~@~!~@~ */
		print_all_symbols(symbol_list);
		print_data_lines_list(data_lines_list);
		

		

}

/*DELETE IT - FOR DEVBUGGING213~@!@~!@#$@#~!!~#!@ */
void print_data_lines_list(head_of_data_lines* list)
{
	data_lines* p;
	p= list->head;
	while(p != NULL)
	{
		printf("\n%u\n", p->data->signal);
		p=p->next;
	}
}
/*DELETE UP!!!! */

void manage_code_lines(char *linePointer)
{
        

	skip_chars(linePointer);

	skipSpaceTab(linePointer);

	if(*linePointer == '\n' || *linePointer == '\0')
		TIC++;
	
        else if(*linePointer == 'r')
        {
                TIC += 2;
                skip_chars(linePointer);
        }

        else if(*linePointer == '#')
        {
                TIC += 3;
                skip_chars(linePointer);
        }
        else{
                TIC+=4;
                skip_chars(linePointer);
        }
        skipSpaceTab(linePointer);

        if(*linePointer == '\n' || *linePointer == '\0')
                return;

        if(*linePointer == ',')
                linePointer++;
        
        skipSpaceTab(linePointer);

       
        if(*linePointer == '#')
        {
                TIC += 1;
                skip_chars(linePointer);
        }
        else if(*linePointer != '\n' && *linePointer != '\0' && *linePointer != 'r')
                TIC += 2;

        

}

void skip_chars(char *linePointer)
{
	while(*linePointer != ' ' && *linePointer != '\n' && *linePointer != '\0' && *linePointer != '\t' && *linePointer != ',')
	{
		memmove(linePointer, linePointer+1, strlen(linePointer));
		continue;
	}
}




void add_extern(head_of_symbol_list* symbol_list ,char *linePointer, int symbol_type, int *error, int line_counter)
{
	char *point_to_label_name;

	skipSpaceTab(linePointer);
	point_to_label_name = save_name(linePointer);

		
        if(point_to_label_name !=NULL)
        {
			if(valid_label(point_to_label_name,error,line_counter))
			{
				if(symbol_list != NULL && symbol_list->head != NULL)
					label_exist(symbol_list, point_to_label_name, error, symbol_type);

				add_label(symbol_list, point_to_label_name, symbol_type);
				linePointer += strlen(point_to_label_name);
				free(point_to_label_name);

				skipSpaceTab(linePointer);

				/*check if there is extraneous text in the line and print an error */
				EXTRANEOUS_TEXT_RETURN
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
    char *name=NULL;
    int length_name=0;
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
        printf("\nError: Memory allocation failed.");
		exit(0);
    }
	/*copying the information to the symbol node */
    strcpy (p->symbol , label_name);

    switch(symbol_type)
    {
        case DATA:
            strcpy(p->attributes, "data");
			p->value = TIC;
    		p->offset = TIC % 16;
			p->base_address = TIC - p->offset;
   			p->next = NULL;
            break;

        case CODE:
            strcpy(p->attributes, "code");
			p->value = TIC;
    		p->offset = TIC % 16;
			p->base_address = TIC - p->offset;
   			p->next = NULL;
            break;

        case ENTRY:
            strcpy(p->attributes, ", entry");
            break;

        case EXTERNAL:
            strcpy(p->attributes, "external");
			p->value = 0;
			p->offset = 0;
			p->base_address = 0;
			p->next = NULL;
            break;

        default: 
            break;
    }
    
    

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



void reading_data_line(head_of_data_lines* data_lines_list ,char *linePointer, int *error, int line_counter)
{
	int lettersCounter = 0;
	int x=0;
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

    while((*linePointer) && ((*linePointer) != '\n') )
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
		else if(*linePointer != '\n' && *linePointer != '\0')
		{
			*error = 1;
			printf("\nError: Missing a comma in line: %d.",line_counter);
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
		
		add_data_line( data_lines_list , add_data_parameter(x) );
		is_Empty=0;
		TIC++;
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



void reading_string_line(head_of_data_lines* data_lines_list ,char *linePointer, int *error, int line_counter)
{
    
			

	/*we need to have space between the command to the parameters
        check if we got undefined directive command, and prints an error if needed */
    UNDEFINED_DIR_CMD
						

	/*we have space so we need to skip till the parameters */
    skipSpaceTab(linePointer);

	if(*linePointer != '"')
	{
		
		printf("\nError: Illegal string, In line: %d.", line_counter);
		*error = 1;
		return;
	}

				

	linePointer++;

	while( *linePointer != '"'){
		if(!string_is_valid(linePointer, error, line_counter))
			return;

		add_data_line(data_lines_list,add_data_parameter(*linePointer) );
		
		TIC++;
		DC++;

		linePointer++;
	}

	add_data_line(data_lines_list,add_data_parameter(0));
	TIC++;
	DC++;

	linePointer++;
	skipSpaceTab(linePointer);
	/*check if there is extraneous text in the line and print an error */
	EXTRANEOUS_TEXT_RETURN
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


/* !~!~!~!~!~!~~!~!~!~!~!~!~ ~!~!~!~!~!~!~ ~!~!~!~!~!~!~ ~!~!~!~!~!~!~ !~!~!~!~!~!~~!~!~!~!~!~!~ ~!~!~!~!~!~!~ ~!~!~!~!~!~!~ ~!~!~!~!~!~!~ */


head_of_symbol_list *create_symbol_head()
 {
     head_of_symbol_list *list = (head_of_symbol_list*)malloc(sizeof(head_of_symbol_list));
     if(list == NULL)
     {
        printf("\nError: Memory allocation failed.");
		exit(0);
     }
     list->head = NULL;
     return list;
 }



head_of_data_lines *create_data_list()
{
        head_of_data_lines *list = (head_of_data_lines*)malloc(sizeof(head_of_data_lines));
        if(list == NULL)
        {
                printf("\nError: Memory allocation failed.");
		exit(0);
        }
        list->head = NULL;
        return list;
}

void add_data_line(head_of_data_lines* data_lines_list , line *data_line)
{
        data_lines *p = (data_lines*)malloc(sizeof(data_lines));
	data_lines* temp;

        if(p==NULL)
        {
                printf("\nError: Memory allocation failed.");
		exit(0);
        }

        p->data = data_line;
        p->next = NULL;

        if(data_lines_list->head == NULL)
    	{
        	data_lines_list->head = p;
        	return;
    	}
    	else
    	{
        	temp = data_lines_list->head;
        	while(temp->next != NULL)
            	        temp = temp->next;
        	temp->next = p;
    	}
}





