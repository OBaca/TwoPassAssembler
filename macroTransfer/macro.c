#include <stdio.h> /*delete later for debug */ 
#include <stdlib.h>
#include <string.h>

#define LENGTH_LINE 81

void macro(char *[]);

/*create_new_macro_file - is a function that gets a file name and change the ending of the name depends on the ending name given. */
char *create_new_macro_file(char *, char *, int);

int name_check(char *, int *, char *);

char *save_name(char *, int *);
void removeSpace(char *, int *);

int main(int argc, char *argv[])
{
	macro(argv);
	return 0;
}

/*This function gets a FILE POINTER ! , DONT FORGET TO CHANGE $$ */
void macro(char *argv[])
{
	FILE *fp, *newfp;
	int i;
	int nameSize = strlen(argv[1]);
	char line[LENGTH_LINE];
	char *macro_name;
	char *macro_table[81];
	int indx_table=0;

	if(!(fp= fopen(create_new_macro_file(argv[1], ".as",nameSize ), "r"))){
		printf("\nError: cannot open file name: %s\n",argv[1]); 
		return;
	}
	if(!(newfp = fopen(create_new_macro_file(argv[1], ".am", nameSize), "w+")))
	{
		printf("\nError: cannot create a file name: %s.am\n",argv[1]); 
		return;
	}	

	
	

	while(fgets(line,LENGTH_LINE , fp) != NULL)	
	{
		for(i=0 ; i<LENGTH_LINE ; i++)
		{
			if(line[i] == ' ' || line[i] == '\t')
				continue;
			if(line[i] != 'm')
			{
				fputs(line, newfp);
				break;
			}
			
			/*The current char must be 'm' */
			if(!( name_check(line, &i, "macro") ))
			{
				fputs(line, newfp);
				break;
			}

			removeSpace(line, &i);
			printf("\n check after space");
			macro_name = save_name(line, &i);
			macro_table[indx_table] = macro_name;
			printf("\n%s, %s",macro_table[indx_table], macro_name);
		} 
		
		
	}
	
	
}


char * save_name(char *line, int *i)
{
	int j;
	int name_length = 0;
	char *name;
	for(j=0 ; line[j] && line[j] != '\n'; j++ )
	{
		name_length++;
	}
	name = (char *)malloc(sizeof(char) * (name_length+1));
	for(j=0; line[j] && line[j] != '\n'; j++)
	{
		name[j] = line[*i];
	}
	name[++j] = '\0';
	return name;
}

int name_check(char *line, int *i, char *name)
{
	int lettersCounter = 0;
	while((line[*i] >= 'A' && line[*i] <= 'Z') || (line[*i] >= 'a' && line[*i] <= 'z') || (line[*i] >= 0 && line[*i] <= 9) || line[*i] == '#' )
	{
		printf("%c - ", line[*i]);
		lettersCounter++;
		(*i)++;
	}
	printf("after loop: %c\n",line[*i]);
	printf("%s, %s", name, line);
	if(strncmp(line, name, strlen(name) ) == 0 )
	{
		printf("equal");
		return 1;
	}
	return 0;
}

void removeSpace(char *line, int *i)
{
	
	int count=0;
	for( ; line[(*i)] ; (*i)++)
	{
		if(line[*i] != ' ' && line[*i] != '\t' && line[*i] != '\n')
			line[count++] = line[*i];
	}
	line[count] = '\0';
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

