#include "constant.h"
#include "data_structure.h"
#include "phaseOne.h"
#include "errors.h"
#include "pre_assembler_phase.h"
#include "manage_line.h"
#include "assembler.h"
#include "phaseTwo.h"








void manage_phaseTwo(essentials *menu, FILE *fp,char *file_name, head_of_data_lines* data_lines_list, head_of_symbol_list *symbol_list, int *error)
{
    FILE *entFile; 
    FILE *obFile;
    FILE *extFile;
    line *final_table[LENGTH_DATA]= {'\0'}; 

    /*lists of the commands in the project */
    char *op1[] = {"clr", "not", "inc", "dec", "jmp", "bne", "jsr", "red", "prn"};
    char *op2[] = {"mov", "cmp", "add", "sub", "lea"};
    char line[LENGTH_LINE]= {'\0'};
    char *linePointer=NULL;
    char label_name[LENGTH_LABEL]= {'\0'};
    char *point_to_label_name = NULL;
    int line_counter=0;
    int label_flag=0;
    int lettersCounter; /*counter to update the pointer for later use. */
    int index=0; /*index to go through the commands arrays */
    int op1_flag=0, op2_flag=0;  /*flag for directive with one operand and two operands */
    char top_ob_line[15]= {'\0'}; /*the head line in the object file */


    entFile = file_open(file_name, "ent",READ_ONLY);
    obFile = file_open(file_name, "ob",READ_ONLY); 
    extFile = file_open(file_name, "ext",READ_ONLY);

    /*prints the head line to the object file */
    sprintf(top_ob_line, "\t\t\t%i\t%i\n",(menu->TIC)-(menu->DC), menu->DC);
    fputs(top_ob_line, obFile);


    while(fgets(line, LENGTH_LINE, fp) != NULL)
    {
        linePointer = line;
        line_counter++;
        lettersCounter=0;
        op1_flag=0;
        op2_flag=0;

           
        /*skipping all the first spaces and tabs */
        skipSpaceTab(linePointer);
        
        /*if its an empty line we skip */
        if(*linePointer == '\n' || *linePointer == '\0')
			continue;

        /*check if we got a comment */
        COMMENT_CHECK

        
        point_to_label_name = is_label(linePointer, &label_flag, error, line_counter);
        
        if(point_to_label_name !=NULL)
        {
            
            strcpy(label_name, point_to_label_name );
            free(point_to_label_name);
			linePointer += strlen(label_name) +1;
        }
        
        skipSpaceTab(linePointer);

        /*we already have .data and .string in the data linked list so we skip them */
        if(word_check(linePointer, ".data", &lettersCounter) || word_check(linePointer, ".string", &lettersCounter) || word_check(linePointer, ".extern", &lettersCounter))
            continue;

        /*if there is an .entry directive command we will check if its valid and add the entries lines to the .ent file */
        if(word_check(linePointer, ".entry", &lettersCounter) )
        {
            linePointer += lettersCounter;

            UNDEFINED_DIR_CMD
        
            skipSpaceTab(linePointer);

            if(*linePointer == '\n' || *linePointer == '\0')
            {
                printf("\nError: No label enterd after an .entry command. In line: %d", line_counter); 
                *error = 1; 
                continue;
            }
            
            point_to_label_name=NULL;

            point_to_label_name = save_name(linePointer);
            
            if(point_to_label_name !=NULL)
            {
                strcpy(label_name, point_to_label_name );
                free(point_to_label_name);
                print_to_entry_file(entFile, symbol_list, label_name, error, line_counter);
                linePointer += strlen(label_name) +1;
            }

            skipSpaceTab(linePointer);

            EXTRANEOUS_TEXT_CONTINUE
            
            continue;
        } 
        
        if(label_flag && (*linePointer == '\n' || *linePointer == '\0'))
        {
                printf("\nError: Label without content. In line: %d",line_counter);
                *error=1;
                continue;
        }
        
        /*directive command with no operands (rts,stop) */
        if(word_check(linePointer, "rts", &lettersCounter) )
        {
            linePointer += lettersCounter;
            skipSpaceTab(linePointer);

            EXTRANEOUS_TEXT_CONTINUE

            final_table[menu->IC] = add_opcode_line(14);
            
            print_obj_line(obFile,final_table[menu->IC], menu->IC);
            menu->IC +=1;
            continue;
        }

        else if(word_check(linePointer, "stop", &lettersCounter) )
        {
             linePointer += lettersCounter;
             skipSpaceTab(linePointer);

             EXTRANEOUS_TEXT_CONTINUE

            final_table[menu->IC] = add_opcode_line(15);
            print_obj_line(obFile,final_table[menu->IC], menu->IC);
            menu->IC +=1;
            continue;
        }
       
        /*check if the command is a directive command with one operand */
        for(index=0 ; index < LENGTH_OP1 ; index++)
        {
            if(word_check(linePointer, op1[index], &lettersCounter))
            {
                linePointer += lettersCounter;
                 
                read_op1_line(menu, obFile,extFile, symbol_list, linePointer, line_counter, error, index);
                op1_flag=1;
            }
        }
        if(op1_flag) 
            continue;
        
        /*check if the command is a directive command with two operands */
        for(index=0 ; index < LENGTH_OP2 ; index++)
        {
            if(word_check(linePointer, op2[index], &lettersCounter))
            {
                linePointer += lettersCounter;
                 
                read_op2_line(menu, obFile,extFile, symbol_list, linePointer, line_counter, error, index);
                op2_flag=1;
            }
        }
        if(op2_flag)
            continue;


        ILLEGAL_DIRECTIVE_CMD
        continue;
    }

    print_all_data_lines(menu ,obFile, data_lines_list);

    if((menu->IC) > LENGTH_DATA)
    {
        printf("\nError: You have passed memory size limit. In line: %d", line_counter);
        *error=1;
    }
    
    fclose(fp);
}






void print_all_data_lines(essentials *menu,FILE *obFile,head_of_data_lines* data_lines_list)
{
    data_lines *temp = data_lines_list->head;

    while(temp != NULL)
    {
        print_obj_line(obFile, temp->data, menu->IC);
        menu->IC +=1;
        temp=temp->next;
    }
}






void read_op1_line(essentials *menu,FILE *obFile ,FILE *extFile, head_of_symbol_list *symbol_list, char *linePointer, int line_counter, int *error, int indexCommand)
{
    enum {CLR, NOT, INC, DEC, JMP, BNE, JSR, RED, PRN };

    line *tempLine = create_single_line();
    symbol_table *symbol;
    
    char operand_name[LENGTH_LINE]= {'\0'};
    char *point_to_operand_name;
    int registerNum;
    int illegal_register_flag=0;
    int illegal_int =0; 
    int lettersCounter=0;
    int num=0;


    skipSpaceTab(linePointer);

    NO_OP_CMD

    switch (indexCommand)
    {
        case CLR:
            print_obj_line(obFile,add_opcode_line(5), menu->IC);
            menu->IC +=1;
            insert_classify(A, tempLine); 
            insert_funct(10, tempLine);          
            break;

        case NOT:
            print_obj_line(obFile, add_opcode_line(5), menu->IC);
            menu->IC +=1;
            insert_classify(A, tempLine);
            insert_funct(11, tempLine);
            break;

        case INC:
            print_obj_line(obFile, add_opcode_line(5), menu->IC);
            menu->IC +=1;
            insert_classify(A, tempLine);
            insert_funct(12, tempLine);
            break;

        case DEC:
            print_obj_line(obFile, add_opcode_line(5), menu->IC);
            menu->IC +=1;
            insert_classify(A, tempLine);
            insert_funct(13, tempLine);
            break;

        case JMP:
            print_obj_line(obFile, add_opcode_line(9), menu->IC);
            menu->IC +=1;
            insert_classify(A, tempLine);
            insert_funct(10, tempLine);
            break;

        case BNE:
            print_obj_line(obFile, add_opcode_line(9), menu->IC);
            menu->IC +=1;
            insert_classify(A, tempLine);
            insert_funct(11, tempLine);
            break;

        case JSR:
            print_obj_line(obFile, add_opcode_line(9), menu->IC);
            menu->IC +=1;
            insert_classify(A, tempLine);
            insert_funct(12, tempLine);
            break;

        case RED:
            print_obj_line(obFile, add_opcode_line(12), menu->IC);
            menu->IC +=1;
            insert_classify(A, tempLine);
            break;
            
        case PRN:
            print_obj_line(obFile, add_opcode_line(13), menu->IC);
            menu->IC +=1;
            insert_classify(A, tempLine);
            break;

        default:
            printf("\nError: Illegal directive command. In line: %d", line_counter);
            *error=1;
            return;
    }

    point_to_operand_name = save_name(linePointer);
    if(point_to_operand_name !=NULL)
    {
        strcpy(operand_name, point_to_operand_name );
        free(point_to_operand_name);
        linePointer += strlen(operand_name);
    }
    
    skipSpaceTab(linePointer);

    EXTRANEOUS_TEXT_RETURN

    /*check if the operand is a register and compatible for his command */
    if(operand_name[0] == 'r' &&( (indexCommand >=0 && indexCommand <= 3) || indexCommand == 8 || indexCommand == 9 ))
    {
        memmove(operand_name, operand_name+1, strlen(operand_name));

        /*check if it is a valid register number */
        if((strlen(operand_name) == 1 && isdigit(operand_name[0])) || (strlen(operand_name) == 2 && isdigit(operand_name[0]) && isdigit(operand_name[1]))  )
        {
            registerNum = atoi(operand_name); 
            
            if(registerNum >= 0 && registerNum <= 15 )
            {
                insert_register(DEST, registerNum, tempLine);
                insert_address(DEST, 3, tempLine);
                
                print_obj_line(obFile, tempLine, menu->IC);
                menu->IC +=1;   
            }
            else illegal_register_flag=1;
        }
        else illegal_register_flag=1;
            
        if(illegal_register_flag)
        {
            ILLEGAL_REGISTER
            return;
        }

        return;
    }

    /*check if the operand is a number and compatible for his command */
    if(operand_name[0] == '#' && indexCommand == 8)
    {
        memmove(operand_name, operand_name+1, strlen(operand_name));

        /*check if the given number is vaild and saves it */
        num = intCheck(operand_name, &illegal_int, &lettersCounter);
		if(illegal_int)
		{
			ILLEGAL_PARAM
		}

        print_obj_line(obFile, tempLine, menu->IC);
        menu->IC +=1;

        tempLine = create_single_line();
        insert_classify(A, tempLine);
        tempLine->signal = num;

        print_obj_line(obFile, tempLine, menu->IC);
        menu->IC +=1;

        return;
    }

    symbol = symbol_compare_type1(symbol_list, operand_name);

    if( symbol != NULL)
    {     
        insert_address(DEST, 1, tempLine);
        print_obj_line(obFile, tempLine, menu->IC);
        menu->IC +=1;

        tempLine = create_single_line();
        if(!strcmp(symbol->attributes, "external"))
        {
            print_to_external_file(extFile, symbol, menu->IC);

            insert_classify(E, tempLine);
            print_obj_line(obFile, tempLine, menu->IC);
            menu->IC +=1;

            tempLine = create_single_line();
            insert_classify(E, tempLine);
            print_obj_line(obFile, tempLine, menu->IC);
            menu->IC +=1;
        }
        else
        {
            insert_classify(R, tempLine);
            tempLine->signal = symbol->base_address;
            print_obj_line(obFile, tempLine, menu->IC);
            menu->IC +=1;

            tempLine = create_single_line();
            insert_classify(R, tempLine);
            tempLine->signal = symbol->offset;
            print_obj_line(obFile, tempLine, menu->IC);
            menu->IC +=1;

        }
        return;
    }

    symbol = symbol_compare_type2(symbol_list, operand_name, error, line_counter, &registerNum);
    if(symbol != NULL)
    {
        insert_address(DEST, 2, tempLine);
        insert_register(DEST, registerNum, tempLine);
        print_obj_line(obFile, tempLine, menu->IC);
        menu->IC +=1;;

        tempLine = create_single_line();
        if(!strcmp(symbol->attributes, "external"))
        {
            print_to_external_file(extFile, symbol, menu->IC);

            insert_classify(E, tempLine);
            print_obj_line(obFile, tempLine, menu->IC);
            menu->IC +=1;

            tempLine = create_single_line();
            insert_classify(E, tempLine);
            print_obj_line(obFile, tempLine, menu->IC);
            menu->IC +=1;
        }
        else
        {
            insert_classify(R, tempLine);
            tempLine->signal = symbol->base_address;
            print_obj_line(obFile, tempLine, menu->IC);
            menu->IC +=1;

            tempLine = create_single_line();
            insert_classify(R, tempLine);
            tempLine->signal = symbol->offset;
            print_obj_line(obFile, tempLine, menu->IC);
            menu->IC +=1;

        }
        return;
    }

    ILLEGAL_OPERAND
}






void read_op2_line(essentials *menu ,FILE *obFile ,FILE *extFile, head_of_symbol_list *symbol_list, char *linePointer, int line_counter, int *error, int indexCommand)
{
    enum {MOV, CMP, ADD, SUB, LEA };

    line *tempLine = create_single_line();
    symbol_table *symbol1=NULL, *symbol2=NULL;

    char operand_name1[LENGTH_LINE]= {'\0'};
    char operand_name2[LENGTH_LINE]= {'\0'};
    char *point_to_operand_name;
    int registerNum;
    int illegal_register_flag=0;
    int errorInInt =0;
    int lettersCounter=0;
    int num1,num2;

    skipSpaceTab(linePointer);

    NO_OP_CMD

    if(*linePointer == ',')
    {
        ILLEGAL_COMMA
    }

    switch (indexCommand)
    {
        case MOV:
            print_obj_line(obFile,add_opcode_line(0), menu->IC);
            menu->IC +=1;
            insert_classify(A, tempLine);          
            break;

        case CMP:
            print_obj_line(obFile, add_opcode_line(1), menu->IC);
            menu->IC +=1;
            insert_classify(A, tempLine);
            break;

        case ADD:
            print_obj_line(obFile, add_opcode_line(2), menu->IC);
            menu->IC +=1;
            insert_classify(A, tempLine);
            insert_funct(10, tempLine);
            break;

        case SUB:
            print_obj_line(obFile, add_opcode_line(2), menu->IC);
            menu->IC +=1;
            insert_classify(A, tempLine);
            insert_funct(11, tempLine);
            break;

        case LEA:
            print_obj_line(obFile, add_opcode_line(4), menu->IC);
            menu->IC +=1;
            insert_classify(A, tempLine);
            break;

        default:
            ILLEGAL_DIRECTIVE_CMD
            return;
    }
    /*save the first operand */
    point_to_operand_name = save_name(linePointer);
    
    if(point_to_operand_name !=NULL)
    {
        strcpy(operand_name1, point_to_operand_name );
        free(point_to_operand_name);
        linePointer += strlen(operand_name1);
    }

    skipSpaceTab(linePointer);

    ILLEGAL_AMOUNT_OP

    if(*linePointer != ',')
    {
        printf("\nError: Missing a comma. In line: %d", line_counter);
        *error=1;
        return;
    }

    linePointer++;

    skipSpaceTab(linePointer);

    if(*linePointer == ',')
    {
        printf("\nError: Too many commas between operands. In line: %d", line_counter);
        *error=1;
        return;
    }

    ILLEGAL_AMOUNT_OP

    /*save the second operand */
    point_to_operand_name = save_name(linePointer);
    
    if(point_to_operand_name !=NULL)
    {
        strcpy(operand_name2, point_to_operand_name );
        free(point_to_operand_name);
        linePointer += strlen(operand_name2);
    }

    skipSpaceTab(linePointer);

    EXTRANEOUS_TEXT_RETURN

    /*check if the operand is a register and compatible for his command for the first operand */
    if(operand_name1[0] == 'r' && (indexCommand >=0 && indexCommand <= 3))
    {  
        memmove(operand_name1, operand_name1+1, strlen(operand_name1));

        if((strlen(operand_name1) == 1 && isdigit(operand_name1[0])) || (strlen(operand_name1) == 2 && isdigit(operand_name1[0]) && isdigit(operand_name1[1]))  )
        {
            registerNum = atoi(operand_name1); 
            
            if(registerNum >= 0 && registerNum <= 15 )
            {
                insert_register(SOURCE, registerNum, tempLine);
                insert_address(SOURCE, 3, tempLine);
                   
            }
            else illegal_register_flag=1;
        }
        else illegal_register_flag=1;
            
        if(illegal_register_flag)
        {
            ILLEGAL_REGISTER
            return;
        }
    }

    /*check if the first operand is a symbol (type1/type2) */
    else if((symbol1 = symbol_compare_type1(symbol_list, operand_name1)) != NULL)
    {
        insert_address(SOURCE, 1, tempLine);
    }
    else if((symbol1 = symbol_compare_type2(symbol_list, operand_name1, error, line_counter, &registerNum)) != NULL)
    {
        insert_address(SOURCE, 2, tempLine);
        insert_register(SOURCE, registerNum, tempLine);
    }

    /*check if the operand is a register or a number and compatible for his command for the first operand */
    else if(((operand_name1[0] == '#' || operand_name1[0] == 'r') && (indexCommand == 4)) || (operand_name1[0] != '#' && indexCommand != 4) || symbol1 == NULL)
    {
        ILLEGAL_OPERAND
    }

    /*check if the operand is a register for the second operand */
     if(operand_name2[0] == 'r' )
    {
        memmove(operand_name2, operand_name2+1, strlen(operand_name2));

        if((strlen(operand_name2) == 1 && isdigit(operand_name2[0])) || (strlen(operand_name2) == 2 && isdigit(operand_name2[0]) && isdigit(operand_name2[1]))  )
        {
            registerNum = atoi(operand_name2); 
            
            if(registerNum >= 0 && registerNum <= 15 )
            {
                insert_register(DEST, registerNum, tempLine);
                insert_address(DEST, 3, tempLine);
                   
            }
            else illegal_register_flag=1;
        }
        else illegal_register_flag=1;
            
        if(illegal_register_flag)
        {
            ILLEGAL_REGISTER
            return;
        }
    }

    /*check if the second operand is a symbol (type1/type2) */
    else if((symbol2 = symbol_compare_type1(symbol_list, operand_name2)) != NULL)
    {
        insert_address(DEST, 1, tempLine);
    }
    else if((symbol2 = symbol_compare_type2(symbol_list, operand_name2, error, line_counter, &registerNum)) != NULL)
    {
        insert_address(DEST, 2, tempLine);
        insert_register(DEST, registerNum, tempLine);
    }

    /*check if the operand is a register or a number and compatible for his command for the second operand */
    else if((operand_name2[0] == '#' && ((indexCommand == 0) || (indexCommand >=2) )) || operand_name2[0] != '#' )
    {
        ILLEGAL_OPERAND
    }
    

    print_obj_line(obFile, tempLine, menu->IC);
    menu->IC +=1;   

    /*inserting additional lines after line 2 depends on the operands types */
    if(symbol1 != NULL)
    {
        tempLine = create_single_line();
        
        if(!strcmp(symbol1->attributes, "external"))
            {
                print_to_external_file(extFile, symbol1, menu->IC);

                insert_classify(E, tempLine);
                print_obj_line(obFile, tempLine, menu->IC);
                menu->IC +=1;

                tempLine = create_single_line();
                insert_classify(E, tempLine);
                print_obj_line(obFile, tempLine, menu->IC);
                menu->IC +=1;
            }
            else
            {
                insert_classify(R, tempLine);
                tempLine->signal = symbol1->base_address;
                print_obj_line(obFile, tempLine, menu->IC);
                menu->IC +=1;

                tempLine = create_single_line();
                insert_classify(R, tempLine);
                tempLine->signal = symbol1->offset;
                print_obj_line(obFile, tempLine, menu->IC);
                menu->IC +=1;
            }   
    }

    
    else if(operand_name1[0] == '#')
    {
        memmove(operand_name1, operand_name1+1, strlen(operand_name1));

        num1 = intCheck(operand_name1, &errorInInt, &lettersCounter);
		if(errorInInt)
		{
			ILLEGAL_PARAM
		}
        tempLine = create_single_line();
        insert_classify(A, tempLine);
        tempLine->signal = num1;

        print_obj_line(obFile, tempLine, menu->IC);
        menu->IC +=1;
    }
    
    
    if(symbol2 != NULL)
    {
        tempLine = create_single_line();
        if(!strcmp(symbol2->attributes, "external"))
            {
                print_to_external_file(extFile, symbol2, menu->IC);

                insert_classify(E, tempLine);
                print_obj_line(obFile, tempLine, menu->IC);
                menu->IC +=1;

                tempLine = create_single_line();
                insert_classify(E, tempLine);
                print_obj_line(obFile, tempLine, menu->IC);
                menu->IC +=1;
            }
            else
            {
                insert_classify(R, tempLine);
                tempLine->signal = symbol2->base_address;
                print_obj_line(obFile, tempLine, menu->IC);
                menu->IC +=1;

                tempLine = create_single_line();
                insert_classify(R, tempLine);
                tempLine->signal = symbol2->offset;
                print_obj_line(obFile, tempLine, menu->IC);
                menu->IC +=1;
            }
    }

    else if(operand_name2[0] == '#')
    {
        memmove(operand_name2, operand_name2+1, strlen(operand_name2));

        num2 = intCheck(operand_name2, &errorInInt, &lettersCounter);
		if(errorInInt)
		{
			ILLEGAL_PARAM
		}
        tempLine = create_single_line();
        insert_classify(A, tempLine);
        tempLine->signal = num2;

        print_obj_line(obFile, tempLine, menu->IC);
        menu->IC +=1;
    }
}






symbol_table *symbol_compare_type1(head_of_symbol_list* list ,char *label_name)
{
    symbol_table *temp = list->head;
    while(temp != NULL)
    {
        if(!strcmp(label_name, temp->symbol) )
        {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}






symbol_table *symbol_compare_type2(head_of_symbol_list* list ,char *label_name,int *error, int line_counter, int *registerNum)
{
    int i=0;
    symbol_table *temp = list->head;

    while(label_name[i] != '[' )
    {
        if(label_name[i] == '\0')
            return NULL;   

        i++;
    }

    while(temp != NULL)
    {
        if(!strncmp(label_name, temp->symbol,i))
        {
                i++;
                if(label_name[i] != 'r')
                {
                    ILLEGAL_REGISTER
                    return NULL;
                }
                memmove(label_name, label_name+(i+1), strlen(label_name));
                i=0;
                while(label_name[i] != ']') 
                {
                    if(label_name[i] == '\0' || !isdigit(label_name[i]) || i>1)
                        return NULL; 
                          
                    i++;
                }
                i++;
                if(label_name[i] != ' ' && label_name[i] != '\t' && label_name[i] != '\0')
                {
                    return NULL;
                }

                label_name[i-1] = '\0';

                *registerNum = atoi(label_name); 

                if(*registerNum <10 || *registerNum >15)
                {
                    return NULL;
                }

                return temp;
        }
        temp = temp->next;
    }

    return NULL;
}






void print_to_external_file(FILE *extFile, symbol_table *symbol, int IC )
{
    char external_base_value[MAX_BASE_LINE_LENGTH]= {'\0'};
    char external_offset_value[MAX_OFFSET_LINE_LENGTH]= {'\0'};
    
    fputs(symbol->symbol, extFile);   

    sprintf(external_base_value, " BASE %i\n", IC);
    sprintf(external_offset_value, " OFFSET %i\n", IC+1);

    fputs(external_base_value, extFile);
    fputs(symbol->symbol, extFile); 
    fputs(external_offset_value, extFile);
    fputs("\n", extFile);
}






void print_to_entry_file(FILE *entFile,  head_of_symbol_list* symbol_list ,char *label_name, int *error, int line_counter)
{
    symbol_table *temp = symbol_list->head;
    
    char entry_values[MAX_ENTRY_LINE_LENGTH]= {'\0'};
    
    while(temp != NULL)
    {
        if( !strcmp(label_name, temp->symbol) )
        {
            if(temp->printed)
                return;
            
            fputs(temp->symbol, entFile);
            
            sprintf(entry_values, ",%i,%i\n", temp->base_address, temp->offset);

            fputs(entry_values,entFile);

            temp->printed = 1; 
            return;
        }
        temp = temp->next;
    }
    
    
    printf("\nError: Label doesn't exist. in line: %d", line_counter);
    *error = 1;
}

