
#include <stdio.h> /*delete later for debug */ 
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define LENGTH_LINE 81
#define LENGTH_MACRO 6
#define LENGTH_LABEL 31
#define LENGTH_ATTRIBUTES 12
#define LENGTH_DATA 8192



/*macros*/
#define COMMENT_CHECK \
    { \
        if(*linePointer == ';') \
            continue; \
    }


/* macros */


#define ILLEGAL_COMMA \
        printf("\nError: Illegal comma. In line: %d", line_counter); \
        *error = 1; \
        return; 
    

#define ILLEGAL_PARAM \
        printf("\nError: Illegal parameter. In line: %d", line_counter); \
        *error = 1; \
        return;
        
#define MULTIPLE_COMMAS \
        printf("\nError: Multiple consecutive commas. In line: %d", line_counter); \
        *error=1; \
        return;
    
#define EXTRANEOUS_TEXT \
        if(*linePointer != '\n' && *linePointer != '\0') \
                {       \
                        printf("\nError: Extraneous text, In line: %d",line_counter); \
                        *error = 1; \
                        return; \
                }



/*define of struct node to store data in the symbol table*/
typedef struct symbol_table
{
        char symbol[LENGTH_LABEL];
        int value;
        int base_address;
        int offset;
        char attributes[LENGTH_ATTRIBUTES];
        struct symbol_table *next;
}symbol_table;


/*struct to store a data/string line  */
typedef struct line
{
	unsigned int classify:4;
	unsigned int signal:16;	
}line;


typedef struct head_of_symbol_list
{
        symbol_table* head;
}head_of_symbol_list;

typedef struct data_lines
{
        line *data;
        struct data_lines *next;
}data_lines;

typedef struct head_of_data_lines
{
        data_lines *head;
}head_of_data_lines;





/*create_new_macro_file - is a function that gets a file name and change the ending of the name depends on the ending name given. */
char *create_new_macro_file(char *, char *, int);

/*name_check - is a function that get two string and compare them. return 1 if they match and 0 otherwise*/
int name_check(char *, char *, int *);

/*skipSpaceTab - is a function that skips spaces and tabs in a string*/
void skipSpaceTab(char *);

/*save_name - is a function that save string into a char pointer*/
char *save_name(char *);

/*removeSpace - is a function that remove all the white spaces from the string*/
void removeSpace(char *);

/*transfer_macro - is a function that gets a pointer to a new file and a line that we check if there is a word that match to one of the saved macros, and a flag, if this is a macro that we saved we transfer the macro lines into the new .am file.  */
void transfer_macro(char [], FILE *, int *);


int valid_label(char *, int *, int);

void label_exist(head_of_symbol_list* ,char *, int *,int );

int string_is_valid(char *, int *, int );




void manage_line(FILE*, head_of_data_lines*, head_of_symbol_list*, int *);

/* */
void manage(char *);

/*get_line, getting a line from a file */
void get_line(FILE*);

/*get_file, opens a file */
FILE* get_file(char *);

/*is_label, if there is label in a line and its valid we will return his name, if not return NULL */
char* is_label(char *, int *, int*, int);

/*add_label, adding a label to the symbol table with his values */
void add_label(head_of_symbol_list* ,char *, int );

/*reading_data_line, reads a line, if there is errors it prints it out, and store data to the data_image */
void reading_data_line(head_of_data_lines*,char *, int *, int );

/*word_check, check if two strings are equal */
int word_check(char *, char *, int *);

/*IntCheck, check if a string of numbers is a valid int, and saves it. */
int intCheck(char *, int *, int *);

void reading_string_line(head_of_data_lines* ,char *, int *, int );

char *createIntChar(char *, int);

int countNum(char *);

void add_extern(head_of_symbol_list*  ,char *, int , int *, int );

void manage_code_lines(char *);

void skip_chars(char *);

/*for debugging please delete it later! !!@~!~!~!~!~!~!~~!~!~!~!~!!~!~~ */
void print_all_symbols(head_of_symbol_list* );
void print_data_lines_list(head_of_data_lines* );
/* !~!~!~!~!~!~~!~!~!~!~!!~!~!~!~!~!~!~!~~!~!~!~!~!!~!~!~!~!~!~!~!~~!~!~!~!~!!~!~*/





/*create_list, creates a linked list */
void create_list();

/*add_data_parameter, adding a data line node */
line *add_data_parameter(int );

head_of_symbol_list *create_symbol_head();

void free_symbol_table_memory(head_of_symbol_list* );


head_of_data_lines *create_data_list();
void add_data_line(head_of_data_lines*  , line *);
void free_data_line_memory(head_of_data_lines* );



enum {DATA, CODE, ENTRY, EXTERNAL};
enum {SOURCE, DEST};
enum {A=4, R=2, E=1};

/*initialize data counter(DC), and instrcution counter(IC) */
int DC = 0;
int IC = 100;

int main(int argc, char *argv[])
{
    
    manage(argv[1]);
    
    return 0;
}

void manage(char* file)
{
    FILE *fp;
	head_of_data_lines *data_lines_list = NULL;
	head_of_symbol_list *symbol_list = NULL;
	int error=0; /*flag to know if there was an error in the program */


    fp = get_file(file);
	

    manage_line(fp, data_lines_list, symbol_list, &error);
	
	
	/*free all the memory in symbol and data list */
	free_symbol_table_memory(symbol_list);
	free_data_line_memory(data_lines_list);
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


void manage_line(FILE *fp, head_of_data_lines* data_lines_list, head_of_symbol_list *symbol_list, int *error)
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
    symbol_list = create_symbol_head();

	data_lines_list = create_data_list();

    
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

	
	/*!~!~!~!~~!~!~ TEMPORARAY FOR DEBUGGING!!@@!@!@~!~!~@~!~@~ */
		print_all_symbols(symbol_list);
		print_data_lines_list(data_lines_list);
		printf("IC: %d, DC: %d", --IC, DC);

		

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
		IC++;
	
        else if(*linePointer == 'r')
        {
                IC += 2;
                skip_chars(linePointer);
        }

        else if(*linePointer == '#')
        {
                IC += 3;
                skip_chars(linePointer);
        }
        else{
                IC+=4;
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
                IC += 1;
                skip_chars(linePointer);
        }
        else if(*linePointer != '\n' && *linePointer != '\0' && *linePointer != 'r')
                IC += 2;

        

}

void skip_chars(char *linePointer)
{
	while(*linePointer != ' ' && *linePointer != '\n' && *linePointer != '\0' && *linePointer != '\t' && *linePointer != ',')
	{
		memmove(linePointer, linePointer+1, strlen(linePointer));
		continue;
	}
}



/* we need to check this function !!~!~!~!~!~!~!~!~!~!*/
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
				EXTRANEOUS_TEXT
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
			p->value = IC;
    		p->offset = IC % 16;
			p->base_address = IC - p->offset;
   			p->next = NULL;
            break;

        case CODE:
            strcpy(p->attributes, "code");
			p->value = IC;
    		p->offset = IC % 16;
			p->base_address = IC - p->offset;
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



void reading_data_line(head_of_data_lines* data_lines_list ,char *linePointer, int *error, int line_counter)
{
	int lettersCounter = 0;
	int x;
	int errorInInt=0;
	int commaFlag =0;
	int is_Empty = 1;

	
    /*we need to have space between the command to the parameters
        check if we got undefined directive command, and prints an error if needed */
    if(*linePointer != ' ') 
        { 
            printf("\nError: Undefined directive command. In line: %d", line_counter); 
            *error = 1; 
            return; 
        } 

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



void reading_string_line(head_of_data_lines* data_lines_list ,char *linePointer, int *error, int line_counter)
{
    
			

	/*we need to have space between the command to the parameters
        check if we got undefined directive command, and prints an error if needed */
    if(*linePointer != ' ') 
        { 
            printf("\nError: Undefined directive command. In line: %d", line_counter); 
            *error = 1; 
            return; 
        } 
						

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
		
		IC++;
		DC++;

		linePointer++;
	}

	add_data_line(data_lines_list,add_data_parameter(0));
	IC++;
	DC++;

	linePointer++;
	skipSpaceTab(linePointer);
	/*check if there is extraneous text in the line and print an error */
	EXTRANEOUS_TEXT
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
void label_exist(head_of_symbol_list* list ,char *label_name, int *error, int symbol_type)
{
    symbol_table *temp = list->head;
    enum {EXTERNAL=3 };

    while(temp->next != NULL)
    {
       

        if(!strcmp(label_name, temp->symbol) )
        {
            
            if(symbol_type == EXTERNAL)
                if(!strcmp(temp->attributes , "external"))
                    return;

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
                    printf("\nError: Missing closing quotation marks in a string command, In line: %d", line_counter);
                else
                    printf("\nError: Not a valid ascii char, In line: %d", line_counter);
                
                *error=1;
                return 0;
            }
    return 1;
} 



char * save_name(char *linePointer)
{
	int j;
	int name_length = 0;
	char *name=NULL;

	/*count the length of the current word in line*/
	for(j=0 ; linePointer[j] && linePointer[j] != '\n' && linePointer[j] != ' ' && linePointer[j] != '\t'; j++ )
	{
		name_length++;
	}

	name = (char *)malloc(sizeof(char) * (name_length+1));
	if(name == NULL)
	{
		printf("Error: Memory allocation failed.");
                exit(0);
	}

	/*copy the word to a "name" string*/
	for(j=0; j < name_length ; j++)
	{
		name[j] = linePointer[j];
	}
	
	name[j] = '\0';
	return name;
}



int name_check(char *linePointer, char *name, int *lettersCounter)
{
	int j=0;
	*lettersCounter =0;
	/*count the chars of the word that the pointer points to*/
	while((linePointer[j] >= 'A' && linePointer[j] <= 'Z') || (linePointer[j] >= 'a' && linePointer[j] <= 'z') || (linePointer[j] >= 0 && linePointer[j] <= 9) || linePointer[j] == '#' || linePointer[j] == '.' )
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



void skipSpaceTab(char *p)
{
	while((*p) != '\n')
	{
		if((*p) == ' ' || (*p) == '\t')
		{
			memmove(p, p+1, strlen(p));
			continue;
		}
		else
			return;
	}
}



void removeSpace(char *linePointer)
{
	int i;
	int count=0;
	for(i=0 ; linePointer[i] ; i++)
	{
		if(linePointer[i] != ' ' && linePointer[i] != '\t' && linePointer[i] != '\n')
			linePointer[count++] = linePointer[i];
	}
	linePointer[count] = '\0'; /*to finish the string*/
}



char *create_new_macro_file(char *oldFileName, char *ending, int originalLength)
{
	/*we store the size of the old file name */
	int oldFileLength = strlen(oldFileName);
	/*if the size is less then the oldFile we need to delete extra chars from the name of file. */
	if(originalLength < oldFileLength)
	{
		oldFileName[oldFileLength - (oldFileLength - originalLength)] = '\0';
	}
	
	strcat(oldFileName, ending);

	return oldFileName;
}










