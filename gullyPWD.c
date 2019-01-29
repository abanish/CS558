//-----------------------------------------------------------------------------
/* CS558 : Systems Lab
 * C program to implement unix pwd command
 * compiled with : gcc -o gullypwd gullyPWD.c
 * version: 1.0.0
 * */
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 128
//-----------------------------------------------------------------------------
int main(int argc,char *argv[])  { 

    //variable to store the current directory of a process

    char currentWorkingDir[BUFFER_SIZE];

    //getcwd system call used to get the directory of the current process

    printf("The process' directory is %s\n",getcwd(currentWorkingDir,BUFFER_SIZE));

    return(EXIT_SUCCESS);
}
//-----------------------------------------------------------------------------