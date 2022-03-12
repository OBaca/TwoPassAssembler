#include "constant.h"
#include "data_structure.h"
#include "manage_line2.h"


line *create_single_line()
{
    line *p = (line*)malloc(sizeof(line));
    if(p==NULL)
    {
            printf("\nError: Memory allocation failed.");
		    exit(0);
    }
    p->classify =0;
    p->signal =0;

    return p;
}


void insert_classify(int classify_type, line *line2)
{
    switch(classify_type)
    {
        case 1:
            line2->classify = line2->classify | (int)pow(2,0);
            break;
        case 2:
            line2->classify = line2->classify | (int)pow(2,1);
            break;
        case 4:
            line2->classify = line2->classify |  (int)pow(2,2);
            break;
        default:

            break;  
    }
}



void insert_funct(int funct_num, line *line2)
{
    switch(funct_num)
    {
        case 10:
            line2->signal = line2->signal | ((int)pow(2,15) + (int)pow(2,13));
            break;
        case 11:
            line2->signal = line2->signal | ((int)pow(2,15) + (int)pow(2,13) + (int)pow(2,12));
            break;
        case 12:
            line2->signal = line2->signal |  ((int)pow(2,15) + (int)pow(2,14));
            break;
        case 13:
            line2->signal = line2->signal |  ((int)pow(2,15) + (int)pow(2,14) + (int)pow(2,12));
            break;
        default:
            break;  
    }
}



void insert_register(int register_type, int register_num, line *line2)
{
    int i = register_type ? 2:8; 
    switch(register_num)
    {
        case 0:
            break;
        case 1:
            line2->signal = line2->signal | (int)pow(2,i);
            break;
        case 2:
            line2->signal = line2->signal | (int)pow(2,i+1);
            break;
        case 3:
            line2->signal = line2->signal | ((int)pow(2,i) + (int)pow(2,i+1));
            break;
        case 4:
            line2->signal = line2->signal | (int)pow(2,i+2);
            break;
        case 5:
            line2->signal = line2->signal | ((int)pow(2,i) + (int)pow(2,i+2));
            break;
        case 6:
            line2->signal = line2->signal | ((int)pow(2,i+1) + (int)pow(2,i+2));
            break;
        case 7:
            line2->signal = line2->signal | ((int)pow(2,i+2) + (int)pow(2,i+1) + (int)pow(2,i));
            break;
        case 8:
            line2->signal = line2->signal | (int)pow(2,i+3);
            break;
        case 9:
            line2->signal = line2->signal | ((int)pow(2,i+3) + (int)pow(2,i));
            break;
        case 10:
            line2->signal = line2->signal | ((int)pow(2,i+1) + (int)pow(2,i+3));
            break;
        case 11:
            line2->signal = line2->signal | ((int)pow(2,i+3) + (int)pow(2,i+1) + (int)pow(2,i));
            break;
        case 12:
            line2->signal = line2->signal | ((int)pow(2,i+3) + (int)pow(2,i+2));
            break;
        case 13:
            line2->signal = line2->signal | ((int)pow(2,i+3) + (int)pow(2,i+2) + (int)pow(2,i));
            break;
        case 14:
            line2->signal = line2->signal | ((int)pow(2,i+3) + (int)pow(2,i+2) + (int)pow(2,i+1));
            break;
        case 15:
            line2->signal = line2->signal | ((int)pow(2,i+3) + (int)pow(2,i+2) + (int)pow(2,i+1) + (int)pow(2,i));
            break;
    }
}


void insert_address(int address_type,int adress_num, line *line2)
{
    int i= address_type ? 0:6;
    switch(adress_num)
    {
        case 0:
            break;
        case 1:
            line2->signal = line2->signal | (int)pow(2,i);
            break;
        case 2:
            line2->signal = line2->signal | (int)pow(2,i+1);
            break;
        case 3:
            line2->signal = line2->signal | ((int)pow(2,i) + (int)pow(2,i+1));
            break;
        default:
            break;
    }
}

void insert_opcode(int command_number, line *line1)
{
    line1->signal = (int)pow(2,command_number);
}

void print_obj_line(FILE* obFile,line *lineOBJ, int IC)
{
    int A=0, B=0, C=0, D=0, E=0;
    line *temp = (line*)malloc(sizeof(line));
    char final_line[20];

    A = lineOBJ->classify;

    temp->signal = lineOBJ->signal;  

    B = temp->signal >> 12 & 0xF;

    temp->signal = lineOBJ->signal;

    C = temp->signal >> 8 & 0xF;

    temp->signal = lineOBJ->signal;

    D = temp->signal >> 4 & 0xF;

    temp->signal = lineOBJ->signal;

    E = temp->signal & 0xF;

    printf("\n A= %d, B=%d, C=%d, D=%d, E=%d\n", A,B,C,D,E);

    if(IC <1000)
        sprintf(final_line, "0%d\tA%x-B%x-C%x-D%x-E%x\n",IC, A, B, C, D, E);
    else
        sprintf(final_line, "%d\tA%x-B%x-C%x-D%x-E%x\n",IC, A, B, C, D, E);

    fputs(final_line,obFile);

    free(temp);
    free(lineOBJ);
}



