//-----------------------------------------------------------------------------
/* CS558 : Systems Lab
 * C program to implement unix mv command
 * compiled using : gcc -o gullymv gullyMV.c
 * executed as: ./gullymv sourceFilename destinationFilename
 * version: 1.0.0
 * */
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

void usage();

//-----------------------------------------------------------------------------
int main(int argc,char* argv[]) {

    /* creates a new link to an existing file with the new location */

    if (link(argv[1],argv[2]) == -1) {
        usage();
        exit(errno);
    }
    
    /* delete the old file name from the filesystem. */

    if (unlink(argv[1]) == -1) {
        perror(argv[1]);
        usage();
        exit(errno);
    }

    return(EXIT_SUCCESS);
}

void usage(){
    printf("Usage : mv <source Filename> <destination Filename>\n");
}
//-----------------------------------------------------------------------------