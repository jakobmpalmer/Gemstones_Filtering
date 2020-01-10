#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /* needed for fork, get pid procedures*/
#include <sys/types.h>
#include <string.h>

int main()
{   
    //printf("\tInside filterColor.c");


char aLine[80];
char ch;
char color[11];
char currColor[11];
char gemName[22];
//char hardness[14];
//char grav[13];
//char refindx[13];
FILE *fpsort;


scanf("%s", color);
 

while((fgets(aLine, 74, stdin)) != NULL)
{
    sscanf(aLine+22, "%s", currColor);

	if (strcmp(color, currColor) == 0) 
	{
		printf("%s", aLine);
	}     
}

//OPEN SORTING FUNCTION
fpsort = popen("sort", "r");

/* connect stdin of sort to fpout*/
if (fpsort == NULL)
{ 
	perror("error in starting sort");
	exit(1);
}/* usefilestreamsfpin,fpoutas I/Ofornameprocessing*/



return 0;
}
