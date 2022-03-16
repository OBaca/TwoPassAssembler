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
#define LENGTH_OP1 9
#define LENGTH_OP2 5

#define READ_ONLY 0
#define W_PLUS 1



enum {DATA, CODE, ENTRY, EXTERNAL};
enum {SOURCE, DEST};
enum {A=4, R=2, E=1};