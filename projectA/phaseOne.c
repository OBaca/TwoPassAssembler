#include "constant.h"
#include "data_structure.h"
#include "phaseOne.h"
#include "errors.h"
#include "macro.h"
#include "manage_line2.h"
#include "assembler.h"


/* initialize data counter(DC), and temp instrcution counter(TIC) */
int DC = 0;
int TIC = 100; /*temp IC */


void manage_phaseOne(FILE *fp, head_of_data_lines* data_lines_list, head_of_symbol_list *symbol_list, int *error, int *external_flag, int *entry_flag)
{ 
	char *linePointer=NULL;
	char *point_to_label_name=NULL;
    char line[LENGTH_LINE]; 
	char label_name[LENGTH_LABEL];
    int label_flag = 0; /*to indicate if there is a label */
    int line_counter=0; 
    int lettersCounter=0; /*to count the letters to update the line pointer  */
    int symbol_type=0; /* to know what type is the symbol ( code / data / external / entry ) */
	int directive_flag = 0; /* flag to know if there is directive command */

    
    while(fgets(line, LENGTH_LINE, fp) != NULL)
    {
		
        label_flag=0;
		directive_flag = 0;
        line_counter++;
        linePointer = line;
		point_to_label_name=NULL;

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
			linePointer += strlen(label_name) +1; /* updating the line pointer */
        }
             
        if(label_flag)
            skipSpaceTab(linePointer);

		if(*linePointer == '.')
			directive_flag = 1;
		
		/*check if the command is data or a string directive. */
        if(word_check(linePointer, ".data", &lettersCounter) || word_check(linePointer, ".string", &lettersCounter))
        {
            symbol_type = DATA;
			
            if(label_flag)
            {
                if(symbol_list != NULL && symbol_list->head != NULL) 
                    label_exist(symbol_list ,label_name  ,error, symbol_type); /*check that there is not two labels with the same name */

                add_label(symbol_list ,label_name, symbol_type, TIC); /* adding the label to the linked list */ 
            }
			
            /* if the command is a .data we save his data in a data linked list */
            if(word_check(linePointer, ".data", &lettersCounter) )
			{	
                linePointer += lettersCounter; /*updating the pointer */
                reading_data_line(data_lines_list ,linePointer, error, line_counter);
            }
            else	/* else this is a string command so we save his values  */
			{ 
                linePointer += lettersCounter; /*updating the pointer */
                reading_string_line(data_lines_list,linePointer, error, line_counter);
            }
            continue;
        }

		
		if(word_check(linePointer, ".entry", &lettersCounter) )
		{
			*entry_flag=1; /*we turn the flag on so we can create an entry file */
			continue;
		}
	
        if(word_check(linePointer, ".extern", &lettersCounter))
        {
			*external_flag=1;	/*we turn the flag on so we can create an external file */
            linePointer += lettersCounter;
            symbol_type = EXTERNAL;

			add_extern(symbol_list, linePointer, symbol_type, error, line_counter);
            
            continue;
        }

		if(directive_flag)
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

            add_label(symbol_list ,label_name, symbol_type, TIC);   
        } 
			
		skipSpaceTab(linePointer);
		
		manage_code_lines(linePointer);      
    }

	fclose(fp);
}


void manage_code_lines(char *linePointer)
{
	skip_chars(linePointer);

	skipSpaceTab(linePointer);

	if(*linePointer == '\n' || *linePointer == '\0')
		TIC++;
        else if(*linePointer == 'r') /* type of operand is a register, no extra lines needed, only the first two */
        {
                TIC += 2;
                skip_chars(linePointer);
        }

        else if(*linePointer == '#') /* type of operand is a number, one extra line needed (number in binary) */
        {
                TIC += 3;
                skip_chars(linePointer);
        }
        else{	/* type of operand is a label, two extra line needed (base,offset) */
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

			add_label(symbol_list, point_to_label_name, symbol_type, TIC);
			linePointer += strlen(point_to_label_name);
			free(point_to_label_name);

			skipSpaceTab(linePointer);

			/*check if there is extraneous text in the line and print an error */
			EXTRANEOUS_TEXT_RETURN
		}
    }
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
	int errorInInt=0; /* flag to know if there was an error in the function intCheck */
	int commaFlag =0; /* flag to know if there was multiple commas between values */
	int is_Empty = 1; /* flag to know if no values in a data line */

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
		
		add_data_line( data_lines_list , add_data_parameter(x) ); /* adding the data line to the linked data list */
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



