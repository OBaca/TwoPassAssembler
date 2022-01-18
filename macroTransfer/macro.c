#include <stdio.h> /*delete later for debug */ 
#include <stdlib.h>
#include <string.h>

#define LENGTH_LINE 81

void macro(char *[]);

/*create_new_macro_file - is a function that gets a file name and change the ending of the name depends on the ending name given. */
char *create_new_macro_file(char *, char *, int);

int name_check(char *, char *, int *);
void skipSpaceTab(char *);
char *save_name(char *);
void removeSpace(char *);

int main(int argc, char *argv[])
{
	macro(argv);
	return 0;
}

/*This function gets a FILE POINTER ! , DONT FORGET TO CHANGE $$ */
void macro(char *argv[])
{
	FILE *fp, *newfp; /*fp pointer to .as, newfp pointer to .am */
	int nameSize = strlen(argv[1]); /*the length of the file name */
	char line[LENGTH_LINE]; /*to store each line in the file */
	char *linePointer; 
	int lettersCounter=0;
	char *macro_table[LENGTH_LINE]; /*macro table - to store macro name and the lines for the macro */
	int indx_table=0;
	int macro_signal=0; /*flag to know if we are inside a macro */
	char macro_array[LENGTH_LINE];
	int indx_macro_array=0;

	/*open file named .as and point to it.  */
	if(!(fp= fopen(create_new_macro_file(argv[1], ".as",nameSize ), "r"))){
		printf("\nError: cannot open file name: %s\n",argv[1]); 
		return;
	}
	
	/*create new file named .am */
	if(!(newfp = fopen(create_new_macro_file(argv[1], ".am", nameSize), "w+")))
	{
		printf("\nError: cannot create a file name: %s.am\n",argv[1]); 
		return;
	}	

	/*reading through the lines in the file */
	while(fgets(line,LENGTH_LINE , fp) != NULL)	
	{
		if(!macro_signal){
			linePointer = line;
			/*skipping tabs and spaces */
			skipSpaceTab(linePointer);

			/*if the next char is not the beginning of "macro" we will copy the whole line to the new file  */
			if(*(linePointer) != 'm')
			{
				fputs(line, newfp);
				break;
			}
			
			/*The current char must be 'm' */
			/*we check if the word is "macro", if not we will copy the whole line to the new file */
			if(!( name_check(linePointer, "macro", &lettersCounter) ))
			{
				fputs(line, newfp);
				break;
			}
			/*now the word must be macro so we updating the pointer to be after macro name. */
			linePointer += lettersCounter;
			lettersCounter=0; /*reset the counter for future use. */
					
			/*if the next char is not a space, we will copy the whole line to the new file */
			if(*(linePointer) != ' ')
			{
				fputs(line, newfp);
				break;
			}
			
			/*skipping spaces and tabs */
			skipSpaceTab(linePointer);

			/*if the next char is the end of the line, the name is not defined, so we copy the whole line to the new file*/
			if(*(linePointer) == '\n' )
			{
				fputs(line, newfp);
				break;
			}
			
			/*saving the name of the macro */
			macro_table[indx_table++] = save_name(linePointer);
			macro_signal = 1;
			printf("\n%s, ",macro_table[indx_table]);
		}
		else
		{
			/*we are inside the box of macro */
			/*skipping spaces and tabs */
			skipSpaceTab(linePointer);
			if(!( name_check(linePointer, "endm", &lettersCounter) ))
			{
				macro_table[indx_table++] = line;
				break;
			}
			lettersCounter=0;
			macro_signal = 0;
			macro_table[indx_table] = '\0';

			macro_array[macro_indx_array++] = macro_table[0];

		}
		
	}
	
	
}


char * save_name(char *linePointer)
{
	int j;
	int name_length = 0;
	char *name;
	for(j=0 ; linePointer[j] && linePointer[j] != '\n' && linePointer[j] != ' ' && linePointer[j] != '\t'; j++ )
	{
		name_length++;
	}

	name = (char *)malloc(sizeof(char) * (name_length+1));

	for(j=0; j < name_length ; j++)
	{
		name[j] = linePointer[j];
	}
	name[++j] = '\0';
	return name;
}

int name_check(char *linePointer, char *name, int *lettersCounter)
{
	int j=0;
	
	while((linePointer[j] >= 'A' && linePointer[j] <= 'Z') || (linePointer[j] >= 'a' && linePointer[j] <= 'z') || (linePointer[j] >= 0 && linePointer[j] <= 9) || linePointer[j] == '#' )
	{
		(*lettersCounter)++;
		j++;
	}
	if(strlen(name) < (*lettersCounter))
		return 0;

	if(strncmp(linePointer, name, strlen(name) ) == 0 )
	{
		printf("\nequal %d, %s", *lettersCounter, linePointer);
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
	linePointer[count] = '\0';
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

