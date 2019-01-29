//--------------------------------------------------------------------------------
/* CS558 : Systems Lab
 * C program to implement unix GREP command
 * compiled as: gcc -o gullygrep gullyGREP.c
 * executed as: ./gullygrep [options] [pattern] filename
 * version: 1.0.0
 * */
//--------------------------------------------------------------------------------

#include <stdio.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h> 
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>   

#define noOfFiles 5

void usage();
char *compare_with_pattern (char *, char *);
char *cmp_case_with_pattern (char *, char *);
char *convert_file_to_tokens (char *, char *, int);

//--------------------------------------------------------------------------------

int main(int argc, char *argv[]) {

    /*array of file descriptor size 10 */
	int 	inputFilePtr[noOfFiles];
    int 	fileSize[noOfFiles];
    int 	i = 0;
	char 	*token;
	char 	*a;	

    /*new line occurred */
	
	char newLine[2] = "\n";

    /*only filename is passed to the function*/

	if(argc == 1) {
		printf("Incorrect usage. Use 'help grep' command for usage\n");
		exit(1);
	}

	else if(argc > 7){
		printf("Maximum number of file supported by this implementation is 10.");
		exit(1);
	}

    /*help required */

	else if(strcmp(argv[1],"-h") == 0 && argv[2] == NULL) {
		usage();
		exit(1);
	}
	
	
    /* match exact words */
	if(strcmp(argv[1], "-w") == 0) {

		char *p;

		/* size of the all the input files */
		for(i = 1; i <= noOfFiles; i++) {

			inputFilePtr[i] = open(argv[i+2], O_RDONLY);	

			fileSize[i] = lseek(inputFilePtr[i], 0, SEEK_END);
		}

		for(i = 1; i <= noOfFiles; i++) {
			a = (char*)malloc(fileSize[i]*sizeof(char));
			lseek(inputFilePtr[i], 0, 0);
			read(inputFilePtr[i], a, fileSize[i]*sizeof(char));

			/* split file content into line tokens */

			token = convert_file_to_tokens(a, newLine, i);
			
			while( token != NULL ) {

				/* compare the each token with the pattern */

				if(p = compare_with_pattern(token, argv[2])) {

					if(isalpha(*(p - 1)) == 0 && isalpha(*(p + strlen(argv[2]))) == 0) {

						/* if only one file is given as argument */

						if(argc < 5)
							printf("%s\n", token);

						/* print filename as well if more than one file */

						else
							printf("%s: %s\n", argv[i+2], token);
					}
				}

				/* get next line of the file */

				token = convert_file_to_tokens(NULL, newLine, i);
			}

			/* close the input file pointer */

			close(inputFilePtr[i]);

			free (a);
		}
	return 0;
    }

	/* compare the argv[1] with -i for ignore case option */

	else if(strcmp(argv[1],"-i") == 0) {

		/* get the size of all the files */

		for(i = 1; i <= noOfFiles; i++) {

			inputFilePtr[i] = open(argv[i+2], O_RDONLY);

			fileSize[i] = lseek(inputFilePtr[i], 0, SEEK_END);
		}

		for(i = 1; i <= noOfFiles; i++) {

			int line_num = 0;

			a = (char*)malloc(fileSize[i]*sizeof(char));
			lseek(inputFilePtr[i], 0, 0);	
			read(inputFilePtr[i], a, fileSize[i]*sizeof(char));

			/* create line tokens of the input file */

   			token = convert_file_to_tokens(a, newLine, i);

 			while( token != NULL ) { 
				line_num++;
				if(cmp_case_with_pattern( token, argv[2]) != NULL) {

					/* if only one file given as argument */

					if(argc < 5)
						printf("%s\n", token);

					/* if more than one file given as argument, print filename */

					else
						printf("%s: %s\n", argv[i+2], token);
					
				}

				token = convert_file_to_tokens(NULL, newLine, i);
  	 		}
			
			/* close input file pointer */

			close(inputFilePtr[i]);
			free (a);
		}
		return 0;
	}
	else if(strcmp(argv[1], "-c") == 0) {

		int count[noOfFiles];

		/* create counter for each file */

		for(i = 0; i < noOfFiles; i++)
			count[i] = 0;

		for(i = 1; i <= noOfFiles; i++) {

			inputFilePtr[i] = open(argv[i+2], O_RDONLY);

			fileSize[i] = lseek(inputFilePtr[i], 0, SEEK_END);
		}

		for(i = 1; i <= noOfFiles; i++) {

			a = (char*)malloc(fileSize[i]*sizeof(char));
			lseek(inputFilePtr[i], 0, 0);	
			read(inputFilePtr[i], a, fileSize[i]*sizeof(char));

			/* convert file contents into line tokens */

   			token = convert_file_to_tokens(a, newLine, i);

 			while( token != NULL ) { 

				/* if there is a match, increment the counter */
				if(compare_with_pattern( token, argv[2]) != NULL) {
					count[i-1]++;
				}
				
				token = convert_file_to_tokens(NULL, newLine, i);
  	 		}
			if(inputFilePtr[i] != -1) {

				/* if the input file number is less than one */

				if(argc < 5)
					printf("%d\n", count[i-1]);

				/* if the input file number is greater than one, print filenames as well */

				else
					printf("%s: %d\n", argv[i+2],count[i-1]);
			}

			/* close the input file pointer */

			close(inputFilePtr[i]);
			free (a);
		}
		return 0;
	}

	/* check if the option is -n to print line number as well */

	else if(strcmp(argv[1], "-n") == 0) {

		for(i = 1; i <= noOfFiles; i++) {
			
			inputFilePtr[i] = open(argv[i+2], O_RDONLY);

			fileSize[i] = lseek(inputFilePtr[i], 0, SEEK_END);
		}

		for(i = 1; i <= noOfFiles; i++) {

			int line_num = 0;
			a = (char*)malloc(fileSize[i]*sizeof(char));
			lseek(inputFilePtr[i], 0, 0);	
			read(inputFilePtr[i], a, fileSize[i]*sizeof(char));

			/* convert the file content into line tokesn */

   			token = convert_file_to_tokens(a, newLine, i);

 			while( token != NULL ) { 
				line_num++;

				if(compare_with_pattern( token, argv[2]) != NULL) {

					if(argc < 5)
						printf("%d:%s\n",line_num, token);
					
					/* print filename if more than one files is being searched */

					else
						printf("%s : %d : %s\n", argv[i+2], line_num, token);
				}

				token = convert_file_to_tokens(NULL, newLine, i);
  	 		}
			
			/* close the input pointer */

			close(inputFilePtr[i]);
			free (a);
		}
		return 0;
	}

	/* prevention condtion where no option is selected */

	else {

		for(i = 1; i <= noOfFiles; i++) {

			inputFilePtr[i] = open(argv[i+1], O_RDONLY);

			fileSize[i] = lseek(inputFilePtr[i], 0, SEEK_END);
		}
		for(i = 1; i <= noOfFiles; i++) {

			a = (char*)malloc(fileSize[i]*sizeof(char));
			lseek(inputFilePtr[i], 0, 0);	
			read(inputFilePtr[i], a, fileSize[i]*sizeof(char));

			/* convert file content into line tokens */

   			token = convert_file_to_tokens(a, newLine, i);

 			while( token != NULL ) { 
				if(compare_with_pattern( token, argv[1]) != NULL) {
					if(argc < 4)
						printf("%s\n", token);

					/* print line number and filename if more than one file is selected */

					else
						printf("%s : %s\n", argv[i+1], token);
				}
				token = convert_file_to_tokens(NULL, newLine, i);
  	 		}

			/* close the input file pointer */

			close(inputFilePtr[i]);
			free (a);
		}
		return 0;
	}
}


void usage () {
	printf("Usage: ./project [OPTION] PATTERN [FILE]...\n Search for PATTERN in each FILE\n");
	printf("  -w       force PATTERN to match only whole words\n");
	printf("  -i       ignore case distinctions\n");
	printf("  -n       print line number with output lines\n");
	printf("  -c       print only a count of matching lines per FILE\n");
	printf("Standard Input : ./project PATTERN\n");
	printf("Standard Input with options : ./project -i/-v/-iv PATTERN\n");
}

char *convert_file_to_tokens(char *a, char *de, int x) {
	static char *c;
	static int j[noOfFiles] = {0, 0, 0, 0, 0};
	static int i[noOfFiles] = {0, 0, 0, 0, 0};
	int p = 0;
	if(a != NULL) {
		c = a;
		while(c[i[x]] != '\0') {
			if(c[i[x]] == de[0])
				c[i[x]] = '\0';
			i[x]++;
		}
		while(c[j[x]] == '\0')
			j[x]++;
		return &c[j[x]];
	}
	else {
		while(j[x] < (i[x]-1))  {
			if(c[j[x]] == '\0' && c[j[x]]+1 != '\0') { 
				j[x]++; 
				return &c[j[x]];
			}
			j[x]++;
		}
		c = NULL;
		return NULL;
	}
}


char *compare_with_pattern (char *a, char *b) {
	char *p = a, *q = b;
	while(*p != '\0' && *q != '\0') {
		if(*p == *q)  {
			p++;
			q++;
		}
		else { 
			if(q == b)
				p++;
			else  {
				p = p - (q- b) + 1;
				q = b;
			}
		}
	}
	if(*q == '\0')
		return p - strlen(b);
	else 
		return NULL;
}

char *cmp_case_with_pattern (char *a, char *b) {
	char *p = a, *q = b;
	while(*p != '\0' && *q != '\0') {
		if((*p == *q) || ((*p - 'a' + 'A') == *q) || ((*p + 'a' - 'A') == *q)) {
			p++;
			q++;
		}
		else { 
			if(q == b)
				p++;
			else  {
				p = p - (q - b) + 1;
				q = b;
			}
		}
	}
	if(*q == '\0')
		return a;
	else 
		return NULL;
}