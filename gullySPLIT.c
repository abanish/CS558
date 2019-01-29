//--------------------------------------------------------------------------------
/* CS558 : Systems Lab
 * C program to implement unix GREP command
 * Compiled with: gcc -o gullystrip gullySTRIP.c
 * Executed as: ./gullystrip [options] [parameter] filename [prefix]
 * version: 1.0.0
 * */
//--------------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <sys/stat.h>

void usage();
char* make_out_file ( char* prefix , int idx );

//--------------------------------------------------------------------------------

int main( int argc, char *argv[]) {

    /*only filename is passed to the function*/

	if(argc == 1) {
		printf("Use 'help strip' command for help and usage.\n");
		exit(1);
	}

    /*help required */

	else if(strcmp(argv[1],"-h") == 0 && argv[2] == NULL) {
		usage();
		exit(1);
	}

    struct stat inputBuffer;

    stat(argv[1],&inputBuffer);

    if(S_ISDIR(inputBuffer.st_mode)){
        printf("%s Its a directoy\n", argv[3]);
        return 0;
    }

    /* compare the second argument with the -l option and implement function of -l */

    if(strcmp(argv[1], "-l") == 0) {
        
        FILE    *inFilePtr;                 // file pointer to the file to be read
        FILE    *outFilePtr;                // file pointer to the file being created
        
        char    line[256];
        char    *outFileName;               // name of the file to be created

        int     outFileIdx      =   1;      // number of file created
        int     lineCount       =   0;      
        int     totalLineCount  =   0;

        int linesPerFile;                   // maximum lines required per output file

        char* outFilePrefix;                // prefix of the name of the outpur files

        sscanf(argv[2], "%d", &linesPerFile);

        /* create the output filename as per format specified in linux */

        if(argv[4]== NULL ){
            outFilePrefix = "x";
        }
        else{
            outFilePrefix = argv[4];
        }

        outFileName =   make_out_file( outFilePrefix, outFileIdx );

        /* open the file to be split in read mode */

        inFilePtr   =   fopen ( argv[3], "r" );

        if ( inFilePtr == NULL ){
            printf( "Error opening the provided file. \n" );
            exit(1);
        }

        /* open the output file in write mode */

        outFilePtr  =   fopen( outFileName, "w");

        if ( outFilePtr == NULL ){
            printf( "Unexpected exception occurred. \n");
            exit(1);
        }

        /* Loop over the input and split it into multiple files.
         */

        while ( fgets ( line, sizeof line, inFilePtr ) != NULL ){
            
            /* number of lines equal to number of lines per file */
            if( lineCount >= linesPerFile ){
                totalLineCount += lineCount;
                lineCount = 0;

                /* the output file is done. close this one */

                fclose ( outFilePtr );
                outFileIdx += 1;

                /* create new output filename */

                outFileName = make_out_file( outFilePrefix , outFileIdx);

                /* open the newly create file to write */

                outFilePtr = fopen(outFileName, "w");

            }

            /* increment line count by 1 */

            lineCount += 1;
            fprintf( outFilePtr, "%s", line );
        }

        /* close the input and output files */

        fclose( outFilePtr );
        fclose ( inFilePtr );

    return 0;
    }

    else if(strcmp( argv[1], "-b") == 0 ){
        
        FILE    *inFilePtr;             // file pointer to the input file to be split
	    FILE    *outFilePtr;            // file pointer to the output files

	    char    line[256];
	    int     outFileIdx      =   1;  // number of output files
        int     fileSize        =   1;  // size of file
        char    c;
        char*   outputFileName;         // name of the output file as per format
        int     maxSize;                // maximum size of the individual files
        char*   outFilePrefix;          // prefix to the output filename
        
        /* read the maximum size of the output files from argument */

        sscanf(argv[2], "%d", &maxSize); 

        /* output prefix is not supplied as argument */

        if(argv[4] == NULL ){
            outFilePrefix = "x";
        }

        /* output prefix is supplied as argument */

        else{
            outFilePrefix = argv[4];
        }
        
        /* open the file to be split in read mode */

	    inFilePtr   =   fopen(argv[3],"r");

	    if (!inFilePtr){  
            printf("Unable to open the given file");
		    return 1;
        }
        
        /* create output file names as per syntax */

        outputFileName = make_out_file( outFilePrefix, outFileIdx );

        /* open the first output file to write */

	    outFilePtr = fopen(outputFileName, "w");

        if ( outFilePtr == NULL ){
            printf( "Unexpected exception occurred. \n");
            exit(1);
        }

        /* start from the first character in the file */

        c=fgetc(inFilePtr);

        while (c!=EOF) {
            
            /* current output file size is equal to the maximum size */

            if (fileSize == maxSize) {

                /* close the current output file pointer */

                fclose(outFilePtr);
                fileSize = 1;
                outFileIdx++;
                outputFileName = make_out_file ( outFilePrefix, outFileIdx );

                /* open a new output file pointer */

                outFilePtr = fopen(outputFileName, "w");

                if (!outFilePtr)
                    return 1;
            }

            putc(c,outFilePtr);

            /* increment file size by one */

            fileSize++;

            /* move to the next character */

            c=fgetc(inFilePtr);

        }

        /* close both input and output file pointers in the end */

        fclose (outFilePtr);
        fclose(inFilePtr);
        return 0;
    }

    /* check if argument supplied  to split i -n */

    else if(strcmp( argv[1], "-n") == 0 ){

        FILE    *inFilePtr;             // pointer to the input file
	    FILE    *outFilePtr;            // pointer to the output file

	    char    line[256];
	    int     outFileIdx      =   1;  // number of output files
        int     fileSize        =   1;  // size of the current output file
        char    c;
        char*   outputFileName;
        int     maxSize;
        int     noOfChunks;
        char*   outFilePrefix;

        sscanf(argv[2], "%d", &noOfChunks);

        if(argv[4]== NULL ){
            outFilePrefix = "x";
        }
        else{
            outFilePrefix = argv[4];
        }

        //printf("no of chunks %d \n", noOfChunks);
        
        /* open the file to be split in read mode */

	    inFilePtr   =   fopen(argv[3],"r");

        fseek(inFilePtr, 0L, SEEK_END);
        int totalFileSize = ftell(inFilePtr);
        rewind(inFilePtr);

        //printf(" file size %d\n",totalFileSize);

        maxSize = totalFileSize/noOfChunks;

        //printf("max size %d", maxSize);

	    if (!inFilePtr){  
            printf("Unable to open the file");
		    return 1;
        }
        
        /* create output file names as per syntax */

        outputFileName = make_out_file( outFilePrefix, outFileIdx );

        /* open the first output file to write */

	    outFilePtr = fopen(outputFileName, "w");

        if (!outFilePtr){
            printf("Unexpected Exception Occurred.");
            return 1;
        }

        /* start at the first character of the input file */

        c=fgetc(inFilePtr);

        while (c!=EOF) {
            
            /* size of the current file is equal to the max size */

            if (fileSize == maxSize) {

                /* close the pointer to the current file */

                fclose(outFilePtr);
                fileSize = 1;

                /* increment number of files created */

                outFileIdx++;

                /* create a new output file */

                outputFileName = make_out_file ( outFilePrefix, outFileIdx );
                outFilePtr = fopen(outputFileName, "w");

                if (!outFilePtr)
                    return 1;
            }

            /* write the character on to the output file */

            putc(c,outFilePtr);

            /* increment the file size */

            fileSize++;

            /* move to the next character */

            c=fgetc(inFilePtr);

        }

        /* close the input and output pointer */

        fclose (outFilePtr);
        fclose(inFilePtr);

        return 0;
    }

    /* check if the argument supplied is -C */

    else if((strcmp( argv[1], "-C") == 0 ) || (strcmp ( argv[1], "-c")==0)){

        FILE    *inFilePtr;             // file pointer to the input file
	    FILE    *outFilePtr;            // file pointer to the output file

	    char    line[256];
	    int     outFileIdx      =   1;  // number of files created
        int     fileSize        =   1;  // size of the current file
        char    c;

        char*   outputFileName;         // name of the current output file
        int     sizeLimit;
        char*   outFilePrefix;          // prefix to the name of the output file

        /* get the size limit for a output file from arguments */

        sscanf(argv[2], "%d", &sizeLimit); 

        /* create output prefix as per format */

        if(argv[4]== NULL ){
            outFilePrefix = "x";
        }
        else{
            outFilePrefix = argv[4];
        }
        
        /* open the file to be split in read mode */

	    inFilePtr   =   fopen(argv[3],"r");

        if (!inFilePtr){  
            printf("unable to open the file");
		    return 1;
        }
        
        /* find out the total size of the file */

        fseek(inFilePtr, 0L, SEEK_END);
        int totalFileSize = ftell(inFilePtr);

        /* go back to the beginning of the file */

        rewind(inFilePtr);

        /* start to read from the first character in the input file */
        
        c=fgetc(inFilePtr);

        while (c!=EOF) {

            /* create output file */

            outputFileName = make_out_file ( outFilePrefix, outFileIdx );
            outFilePtr = fopen(outputFileName, "w");

            if (!outFilePtr){
                printf("Unexpected Exception Occurred.");
                return 1;
            }
            
            /* copy the complete input file to output file if the file size limit is more */

            if(totalFileSize <= sizeLimit){

                while (c!=EOF){
                    putc(c,outFilePtr);
                    c=fgetc(inFilePtr);
                }

                /* close the file pointers and exit split */

                fclose (outFilePtr);
                fclose(inFilePtr);
                exit(1);
            }

            /* if the file size is more than size limit */

            else{
                fileSize = 1;
                do
                {   
                    /* create a output file less than size limit */
                    putc(c,outFilePtr);
                    fileSize++;
                    c=fgetc(inFilePtr);                 
                }while(fileSize != sizeLimit);

                /* increment the number of files created */

                outFileIdx = outFileIdx + 1;
                totalFileSize = totalFileSize - sizeLimit;
    
            }
        }

        /* close the input and output file pointers */
        fclose (outFilePtr);
        fclose(inFilePtr);
        return 0;
    }

    else if (argc == 2){

        FILE    *inFilePtr;             // file pointer to the input file
        FILE    *outFilePtr;            // file pointer to the output file

        char    line[256];
        char    *outFileName;           // name of the output file

        int     outFileIdx      =   1;


        char* outFilePrefix;            // prefix of the output file name as required
        
        /* set prefix x as no argument supplied for prefix */

        outFilePrefix = "x";

        outFileName =   make_out_file( outFilePrefix, outFileIdx );

        /* open the file to be split in read mode */

        inFilePtr   =   fopen ( argv[1], "r");

        if ( inFilePtr == NULL ){
            printf("Error opening the file \n");
            exit(1);
        }

        /* open the output file in write mode */

        outFilePtr  =   fopen( outFileName, "w");

        if ( outFilePtr == NULL ){
            printf("Unexpected Exception \n");
            exit(1);
        }

        /* Loop over the input and copy to the output file.
         */

        while ( fgets ( line, sizeof line, inFilePtr ) != NULL ){
            fprintf( outFilePtr, "%s", line );
        }
        
        /* close the input and output file pointers */
        fclose( outFilePtr );
        fclose ( inFilePtr );

    return 0;

    }
}

/* print usage of the split command of the gully shell */

void usage () {
	printf("Usage: ./project [OPTION][FILE [PREFIX]]...\n split - split a file into pieces\n");
	printf("\t\t-l, --lines=NUMBER \t put NUMBER lines/records per output file\n");
	printf("\t\t-b, --bytes=SIZE \t put SIZE bytes per output file\n");
	printf("\t\t-n, --number=CHUNKS \t generate CHUNKS output files; see explanation below\n");
	printf("\t\t-C  --put at most SIZE bytes of records per output file\n");
}

/* create the output file name as per format */

char* make_out_file ( char* prefix, int idx ){
    char *outFileNm;
    int count = 0;
    for( char l = 'a'; l<='z'; ++l) {
        for (char m = 'a'; m<='z'; ++m ){
            count++;
            if(count == idx ){
                char str1[2];
                char str2[2];
                
                /* append prefix and suffix to the new output file name */

                char * str3 = (char *) malloc(1 + strlen(str1)+ strlen(str2)+ strlen(prefix) );

                /* conver characters to string */

                str1[0] = l;
                str1[1] = '\0';
                str2[0] = m;
                str2[1] = '\0';

                strcpy(str3, prefix);
                strcat(str3, str1);
                strcat(str3, str2);
                
                return str3;
            }

        }
    }
}


