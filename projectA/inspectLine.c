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
    printf("\nWE Exit manage line totally");
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

    printf("\nBefore fgets\n");
    
    while(fgets(line, LENGTH_LINE, fp) != NULL)
    {
        printf("\nLine: %d",line_counter);
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


        if(word_check(linePointer, ".data", &lettersCounter) || word_check(linePointer, ".string", &lettersCounter))
        {
            
            symbol_type = DATA;

            if(label_flag)
            {
                label_exist(label_name  ,&error);
                
                add_label(label_name, symbol_type);
            }

            
            if(word_check(linePointer, ".data", &lettersCounter) ){
                linePointer += lettersCounter; /*updating the pointer */
                reading_data_line(linePointer, &error, line_counter);
                printf("\nEnterd DATA reading line\n");
            }
            else{
                linePointer += lettersCounter; /*updating the pointer */
                reading_string_line(linePointer, &error, line_counter);
                printf("\nEnterd else STRING reading line\n");
            }
            printf("\ncontinue");
            continue;
        }

        
            printf("\n END");
            

        

        
                
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
        if(valid_label(name, error, line_counter))
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
     
	symbol_table* temp;
	/*copying the information to the symbol node */
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

        printf("\n%u ", data_image[IC]->signal);
		data_image[IC] = add_data_parameter(x);
		is_Empty=0;
		printf("\n%u ", data_image[IC]->signal);
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
    printf("\nEnterd string_line reading");

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
		printf("\n %d", data_image[IC]->signal);
		IC++;
		DC++;

		linePointer++;
	}

	data_image[IC] = add_data_parameter(0);
	printf("\n %d", data_image[IC]->signal);
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
