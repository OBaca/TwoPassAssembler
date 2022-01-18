#include <stdio.h>

typedef struct line2 {
	unsigned int classify:4;
	unsigned int funct:4;
	unsigned int ogerMakor:4;
	unsigned int mieonMakor:2;
	unsigned int ogerYaad:4;
	unsigned int mieonYaad:2;
	int lineIdentity = 2;
	}line2;
line2 bytes;

typedef struct line1 {
	unsigned int classify:4;
	unsigned int signal:16;	
	int lineIdentity = 1;
	}line1;
line1 opcode;



void convert_manage(void* ptr, int lineKind);
void convert_line2_to_OB(line2);
void convert_line1_to_OB(line1);


int main(){
/*~!~ paramaters for the function to convert final result */
	unsigned int a;
	int c;
	int d;
	int mask;
	
	/* defnition of the struct */
	bytes.classify = 4;
	bytes.funct = 10;
	bytes.ogerMakor = 3;
	bytes.mieonMakor = 1;
	bytes.ogerYaad = 6;
	bytes.mieonYaad = 3;
	printf("\n1) %d.\n2) %d.\n3) %d.\n4) %d.\n5) %d.\n6) %d.\n", 			bytes.classify, bytes.funct ,bytes.ogerMakor, bytes.mieonMakor, 		bytes.ogerYaad,bytes.mieonYaad  );	
	
/*~!~ How to convert normal struct to final result with letters 
	-Convert the 4 most right bytes aka D*/

	c = bytes.mieonMakor << 2;
	d = bytes.ogerYaad >> 2;
	a = c | d;
	printf("\nc)%d\nd) %d\na) %d",c,d,a);

/*DONT TOUCH */
  	printf("\n");
	
/*How to convert normal struct to final result with letters
	-Convert the 4 most right bytes aka E */
	c = bytes.mieonYaad;  
	mask = 8;	      
	d = bytes.ogerYaad & (~mask); 
	printf("\n mask = %d, d = %d",mask, d);
	mask =4;
	d = d & (~mask);
	printf("\n mask = %d, d = %d, c= %d",mask, d,c);
	d = d<<2;
	a = c | d;	
	printf("\nc)%d\nd) %d\na) %d",c,d,a);

/*send a struct to a function and check the signal of the byte (the first line ) */
	opcode.classify = 4;
	opcode.signal = 4;	
	
	convert_manage(&line1, 1);
	
	return 0;
}

void convert_manage(void* ptr, int lineKind) {
    switch (lineKind) {
        case 1:
            line1 *p1 = (line1*)ptr;
	    printf("\nThis is line1");
            // Print p1->a, p1->b, and so on
            break;
        case 2:
            line2 *p2 = (line2*)ptr;
		printf("\nLine2");
            // Print p2->a, p2->b, and so on
            break;
    }
}




