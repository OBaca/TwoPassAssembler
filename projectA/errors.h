/* macros */
#define UNDEFINED_DIR_CMD \
    if(*linePointer != ' ' && *linePointer != '\t') \
        { \
            printf("\nError: Undefined directive command. In line: %d", line_counter); \
            *error = 1; \
            return; \
        } 

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


extern symbol_table *symbol_head;


int valid_label(char *, int *, int);

void label_exist(head_of_symbol_list* ,char *, int *,int );

int string_is_valid(char *, int *, int );

