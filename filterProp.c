#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/types.h>


int main()
{   

char aLine[80];
char gemName[22];
double hardness;
double hardLow, hardHigh;
double grav, gravLow, gravHigh;

//printf("\tFiltering Prop. . . .\n");
//     scanf("%f %f", &hardLow, &hardHigh);
//     printf("HardLow-high [ %f - %f ] \n", hardLow, hardHigh);
//      scanf("%f %f", &gravLow, &gravHigh);
//     printf("GravLow-high [ %f - %f ] \n", gravLow, gravHigh);
                 
while((fgets(aLine, 74, stdin)) != NULL)
{ 
    sscanf(aLine+33, "%lf", &hardness);
    //sscanf(aLine+47, "%f", &grav);
    //printf("Hardness = %f, Grav = %f\n", &hardness, &grav);                
    //printf("Hardness = %f\n", hardness);
    if ((hardness > hardLow) && (hardness < hardHigh) ) /* Check Hardness */
    {
        printf("%s", aLine);
        // if((grav > gravLow) && (grav < gravHigh)){ /* Check gravity */
        //     printf("%s", aLine);
            //printf("HARDNESS %f \n", hardness);
        // }   
    }

}


return 0;
}

