#include "manage_line2.h"
#include "data_structure.h"
#include "constant.h"


void insert_classify(int classify_type, line *line2)
{
    switch(classify_type)
    {
        case 16:
            line2->signal = line2->signal | (int)pow(2,16);
            break;
        case 17:
            line2->signal = line2->signal | (int)pow(2,17);
            break;
        case 18:
            line2->signal = line2->signal |  (int)pow(2,18);
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
        case 3:
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
