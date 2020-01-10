#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /* needed for fork, get pid procedures*/
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

/*Prototypes */
//void StartCat();
//void FilterColor();



//CONTROL PANEL
int main(){

/*VARS*/
int fd[2]; /* provde file descr pointer array for pipe in/out */
int fdp[2];
pid_t pid1,pid2; /* processid's for each child*/
FILE*fpin,*fpout;/* file variablesnfor mainprocess I/O*/
/*----------------------- */
//int value,value1; /* integer values for writing and reading*/

printf("running...\n");

char color[7];
double hardLow;
double hardHigh;
double gravLow;
double gravHigh;

/* reads input (color desired, lower and upper bounds for desired hardness, 
lower and upper bounds for specific gravity) from stdin */
printf("Enter: colorFilter\n");
//fflush(stdout);
scanf("%s", color);

printf("Enter Hardness Bounds");
scanf("%lf %lf", &hardLow, &hardHigh);

printf("Enter Gravity Bounds");
scanf("%lf %lf", &gravLow, &gravHigh);
//fflush(stdout);
//scanf("%s", color, 7, stdin);
// scanf("%s", color, stdin);
// printf( "Lower (Hardness) Bound = \n");
// scanf("%s",hardLow, 5, stdin);
// printf("Upper (Hardness) Bound = \n");
// scanf("%s", hardHigh, 5, stdin);
// printf("Lower (Gravity) Bound?");
// fgets(gravLow, 5, stdin);
// printf("Upper (Gravity) Bound");
// fgets(gravHigh, 5, stdin);
printf("Your Color to filter is: %s\n Between %lf - %lf Hardness\n Between %lf - %lf Gravity\n\n", color, hardLow, hardHigh, gravLow, gravHigh);


//printf("Your filter is: Color = %s | Hardness = [%s - %s] | Gravity = [%s - %s]", color, hardLow, hardHigh, gravLow, gravHigh);

/* ------------------------------------------------------------------
Cat File In 
 ------------------------------------------------------------------ */

/* create pipe and check for an error*/
if (pipe(fd)< 0)
{ 
    perror("pipe error!+!+!");
    exit(1);
}

if(pipe(fdp)<0)
{
    perror("Pipe error!+");
    exit(1);
}

if ((pid1= fork())< 0) /* apply fork and check for error*/
{ 
    perror("error in fork!+!+!");
    exit(1);
}

/* ------------------------------------------------ 
  First child
 ------------------------------------------------ */

if (0 == pid1)
{ /* processingforchild*/
close(fd[1]);/* close output end, leaving input open*/
    if (fd[0]!=STDIN_FILENO) /* set standard input to pipe*/
    {
        if (dup2(fd[0],STDIN_FILENO)!= STDIN_FILENO)
        { 
          perror("dup2errorforstandardinput: ChildOne");
        exit(1);
        }
      close(fd[0]); /* notneededafterdup2*/
    }
    
    execlp("./filterProp","./filterProp",(char*) 0);
    //printf("exiting. . .");
    //exit(1);

      
} else { /* processingforparent*/
    printf("The parent process continues.\n");/* spawn second process*/


/* ------------------------------------------------------------------
Second Child
 ------------------------------------------------------------------ */

        if ((pid2= fork())< 0)/* apply fork again for second child*/
        { 
            perror("errorin fork");
            exit(1);
        }

        if (0 == pid2)
        { /* processing for child*/
            printf("The Cat child process is active.\n");
            close(fd[0]);/* close input end,leaving output open*/                        
            if (fd[1]!=STDOUT_FILENO)   /* set standard output to pipe*/
            { 
                if (dup2(fd[1],STDOUT_FILENO)!= STDOUT_FILENO)
                { 
                    perror("dup2errorforstandardoutput:ChildTwo");
                    exit(1);
                }
                        close(fd[1]);/* not needed after dup2*/
            }            
            execlp("cat", "cat", "./gems.txt",
            (char*) 0);/* print to the pipe, standard input*/

/* ------------------------------------------------------------------
Parent
 ------------------------------------------------------------------ */
        } else{ /* processing continues for parent*/

                
                close(fdp[1]);/* leaving INPUT open*/                        
                if (fdp[0]!=STDOUT_FILENO)   /* set standard output to pipe*/
                { 
                    if (dup2(fdp[0],STDOUT_FILENO)!= STDOUT_FILENO)
                    { 
                        perror("dup2errorforstandardoutput:Parent MainControl");
                        exit(1);
                    }
                    close(fdp[0]);/* not needed after dup2*/
                }


                printf("Filtering Color\n");
                fpout = popen("./filterColor", "w"); /* Popen file out -> stdin */
                if (fpout == NULL)
                { 
                perror("!!! error in filtering color !!!");
                exit(1);
                }



perror("\n\nmadeit!\n\n");
                /*--------------------------
                Reading
                ----------------------------*/   
                //printf("\t\tREADING\n");

                close(fdp[0]);/* Leaving OUTPUT open*/                        
                if (fdp[1]!=STDOUT_FILENO)   /* set standard output to pipe*/
                { 
                if (dup2(fdp[1],STDOUT_FILENO)!= STDOUT_FILENO)
                { 
                    perror("dup2errorforstandardoutput:Parent MainControl");
                    exit(1);
                }
                close(fdp[1]);/* not needed after dup2*/
                }


                char aLine[80];

                fprintf(fpout, "%s", color); // setup color
                printf("%lf %lf %lf %lf", hardLow, hardHigh, gravLow, gravHigh); // setup hardness
                //printf("%f %f", ); // setup gravity
                
                while((fgets(aLine, 80, stdin)) != NULL)
                { 
                    fprintf(fpout, "%s", aLine);
                    printf("%s", aLine);
                }
                printf("\t\tFINISHED READING \n");
                   

                

/* ------------------------------------------------------------------
Cleanup
 ------------------------------------------------------------------ */
                printf("Parent closing its pipe ends: parent does not use pipe\n");
                close(fd[0]);
                close(fd[1]);
                close(fdp[0]);
                close(fdp[1]);
                printf("Parent waits for both children to finish.\n");
                waitpid(pid1,NULL,0);/* waitforfirstchildto finish*/
                waitpid(pid2,NULL,0);/* waitforsecondchildto finish*/
                // if (fclose(fpin))
                // { 
                //     perror("error in child closing fpin");
                //     exit(1);
                // }
                if (pclose(fpout))
                { 
                    perror("error in child closing fpout");
                    exit(1);
                }
                printf("Parent finished.\n");
                }
  }

printf("End Main");

return 0;
} //Main
