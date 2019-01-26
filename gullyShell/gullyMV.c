/* CS558 : Systems Lab
 * C program to implement unix mv command
 * version: 1.0.0
 * */
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
//-----------------------------------------------------------------------------
int main(int argc,char* argv[]) {

    if (link(argv[1],argv[2]) == -1) {
        perror(argv[1]);
        perror(argv[2]);
        exit(errno);
    }

    if (unlink(argv[1]) == -1) {
        perror(argv[1]);
        exit(errno);
    }

    return(EXIT_SUCCESS);
}
//-----------------------------------------------------------------------------