/* CS558 : Systems Lab
 * C program to implement unix pwd command
 * version: 1.0.0
 * */
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 128
//-----------------------------------------------------------------------------
int main(int argc,char *argv[])  { 

    char CWD[BUFFER_SIZE];

    printf("The process' directory is %s\n",getcwd(CWD,BUFFER_SIZE));

    return(EXIT_SUCCESS);
}
//-----------------------------------------------------------------------------