#ifndef ERRORS_H
#define ERRORS_H

/* macros for common errors */
#define ILLEGAL_COMMA \
        printf("\nError: Illegal comma. In line: %d", line_counter); \
        *error = 1; \
        return; 
    
#define ILLEGAL_OPERAND \
        printf("\nError: Illegal operand. In line: %d", line_counter); \
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

#define ILLEGAL_DIRECTIVE_CMD \
        printf("\nError: Illegal directive command. In line: %d", line_counter); \
        *error=1;

#define MEMORY_ALLOCATION_FAIL \
        printf("\nError: Memory allocation failed."); \
	exit(0);

#define ILLEGAL_REGISTER \
        printf("\nError: Illegal register name. In line: %d",line_counter); \
        *error=1;
    
#define EXTRANEOUS_TEXT_RETURN \
        if(*linePointer != '\n' && *linePointer != '\0') \
        {       \
                printf("\nError: Extraneous text, In line: %d",line_counter); \
                *error = 1; \
                return; \
        } 

#define EXTRANEOUS_TEXT_CONTINUE \
        if(*linePointer != '\n' && *linePointer != '\0') \
        {  \
                printf("\nError: Extraneous text, In line: %d",line_counter); \
                *error = 1; \
                continue;   \
        }

#define UNDEFINED_DIR_CMD \
        if(*linePointer != ' ' && *linePointer != '\t') \
        { \
                printf("\nError: Undefined directive command. In line: %d", line_counter); \
                *error = 1; \
                return; \
        } 

#define NO_OP_CMD \
        if(*linePointer == '\n' || *linePointer == '\0') \
        { \
        printf("\nError: Directive command with no operands. In line: %d",line_counter); \
        *error=1; \
        return; \
        }

#define ILLEGAL_AMOUNT_OP \
        if(*linePointer == '\n' || *linePointer == '\0') \
        { \
        printf("\nError: Amount of operands doesn't match the directive command. In line: %d", line_counter); \
        *error=1; \
        return; \
        }


extern symbol_table *symbol_head;

/*valid_label, check if the name of the label is legal */
int valid_label(char *, int *, int);

/*label_exist, checks if there is already a symbol with the same name in our symbol table */
void label_exist(head_of_symbol_list* ,char *, int *,int );

/*string_is_valid, gets a string from user and check if the chars inside it are legal */
int string_is_valid(char *, int *, int );

#endif
