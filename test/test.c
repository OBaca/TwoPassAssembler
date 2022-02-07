#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int name_check(char *, char *, int *);


int main()
{
    int p = 0;
    char word[10] = ".data! ";
    char *wordP = word;
   if(name_check(wordP, ".data", &p)){
       wordP += p;
        printf("\n HI");
        
   }
    return 0;

}

int name_check(char *p, char *name, int *lettersCounter)
{
	int j=0;
	
	/*count the chars of the word that the pointer points to*/
	while((p[j] && p[j] != ' ' && p[j] != ',' && p[j] != '\t' && p[j] != '\n' ))
	{
        printf("\n%c, %d", p[j], *lettersCounter);
		(*lettersCounter)++;
		j++;
        
	}
	/*checking if the word is bigger then it should be */
	if(strlen(name) < (*lettersCounter))
		return 0;
	
	if(strncmp(p, name, strlen(name) ) == 0 )
	{
		return 1;
	}
	return 0;
}