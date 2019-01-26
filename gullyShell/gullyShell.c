//----------------------------------------------------------------------------------------
/* CS558: Systems Lab: Assignment 1
* Write a C program to simulate a bash shell of your own say “my_shell” 
* which consists of Linux commands:
*    1. pwd
*    2. mv
*    3. grep
*    4. split
*  
*  Name : Gully Shell
*  Version : 1.0.0
*  Edits: 
*  Release: 260119
*/
//----------------------------------------------------------------------------------------

#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include<readline/readline.h> 
#include<readline/history.h> 
#include<limits.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LIST_SIZE 1000 // max number of letters to be supported 
#define MAX_CMD_SIZE 100 // max number of commands to be supported 
#define HISTORY_MEM_SIZE 50

//----------------------------------------------------------------------------------------

void shell_looper();
static void dump_cmd(char **argv);

/* clear terminal screen */

void clrscr()
{
    system("@cls||clear");
}

int main( int argc, char **argv ){

    clrscr();

    /* load all the shell configuration files */

    printf("\t   !!!!!!!!!!! WELCOME TO THE GULLY SHELL !!!!!!!!!!\n ");
    printf("_______________________________________________________________________\n\n");
    
    /* run Read Evaluate Print Loop indefinitely */

    shell_looper();

    /* perform shutdown or any cleanup */

    return 0;
}

/* display a linux like prompt and read the input */

char* read_prompt(){

    char    hostname[200];
    char    cwd[50];
    char    *usrName;
    char    *readBuffer;
    char    *prompt;
    char    *home;

    readBuffer  =   malloc(MAX_CMD_SIZE*sizeof(char)); 
    usrName     =   (char *)malloc(20*sizeof(char));

    /* get the name of the system from builtin libs */

    gethostname(hostname, 200);

    /* get the current working directory */

    getcwd(cwd, sizeof(cwd));

    /* get the current user name */

    usrName=getlogin();

    /* configure prompt similar to the ubuntu terminal */

    prompt  =   strcat(usrName,"@");
    prompt  =   strcat(prompt,hostname);
    prompt  =   strcat(prompt,":~");
    prompt  =   strcat(prompt,cwd);
    prompt  =   strcat(prompt, "$ ");

    /* read user inputs from the console */

    readBuffer = readline(prompt);

    return readBuffer;
}

/* parse command line input strings into cmd tokens */

char **parse_input(char *input) {

    char    **commandTokens  =   malloc(8 * sizeof(char *));
    char    *separator =   " ";
    char    *parsed;
    int     index      =    0;

    parsed = strtok(input, separator);
    while (parsed != NULL) {
        commandTokens[index] = parsed;
        index++;
        parsed = strtok(NULL, separator);
    }
    
    /* last command argument as NULL */

    commandTokens[index] = NULL;
    return commandTokens;
}

/* function where the gully shell implemented commands are executed */

void performCmndExecution(char** parsed) 
{ 
    // Forking a child 

    pid_t pid = fork();  
  
    if (pid == -1) { 
        printf("\nFailed forking child.."); 
        return; 
    } 
    else if (pid == 0) { 
        if (execvp(parsed[0], parsed) < 0) { 
            printf("\nCould not execute command.."); 
        } 
        exit(0); 
    } 
    else { 

        // waiting for child to terminate 

        wait(NULL);  
        return; 
    } 
} 

/* function to handle user input and and helper to execution of implemented commands
 * in the this shell 
 * */

int executeCmndHandler(char** parsedArgs) 
{ 
    int     noOfCmdsSupported = 4;
    int     i;
    int     switchValue = 0; 
    char    *supportedCmnds[noOfCmdsSupported]; 
    char    *username;  
    
    /* assign indexes to the commands of the shell */

    supportedCmnds[0] = "pwd"; 
    supportedCmnds[1] = "mv"; 
    supportedCmnds[2] = "grep"; 
    supportedCmnds[3] = "split"; 
    
    /* find correct index of the command */

    for (i = 0; i < noOfCmdsSupported; i++) { 
        if (strcmp(parsedArgs[0], supportedCmnds[i]) == 0) { 
            switchValue = i + 1; 
            break; 
        } 
    }
    
    /* execute the supplied command */

    switch (switchValue) { 

        /* execution of pwd implemented in C 
         * runs object file name: gullypwd
         * C file name: gullyPWD.c
         * compiled using :- gcc -o gullypwd gullyPWD.c
         */

        case 1:{ ;
                    char *args[]={"./gullypwd",NULL}; 
                    performCmndExecution(args);
                    return 1;
        }

        /* execution of mv implemented in C 
         * runs object file name: gullymv
         * C file name: gullyMV.c
         * compiled using :- gcc -o gullymv gullyMV.c
         */

        case 2: { ;     
                    char *cmndtkn[MAX_LIST_SIZE]; 
                    for(i = 0; i< MAX_CMD_SIZE; i++){
                        if(parsedArgs[i]==NULL)
                            break;
                        cmndtkn[i]=parsedArgs[i];
                    }
                    char *source = cmndtkn[1];
                    char *destination = cmndtkn[2];
                    char *argp[]={"./gullymv",source, destination,NULL};
                    performCmndExecution(argp);
                    return 1;
        }

        /* execution of grep implemented in C 
         * runs object file name: gullygrep
         * C file name: gullyGREP.c
         * compiled using :- gcc -o gullygrep gullyGREP.c
         */

        case 3:{ ;
                    char *cmndtkn2[MAX_LIST_SIZE];
                    for(i = 1; i< MAX_CMD_SIZE; i++){
                        if(parsedArgs[i]==NULL)
                            break;
                        cmndtkn2[i]=parsedArgs[i];
                    }
                    cmndtkn2[i] = NULL;
                    cmndtkn2[0] = "./gullygrep";
                    performCmndExecution(cmndtkn2);
                    return 1;
        }

        /* execution of split implemented in C 
         * runs object file name: gullysplit
         * C file name: gullySPLIT.c
         * compiled using :- gcc -o gullysplit gullySPLIT.c
         */

        case 4: {
                    printf("split implementation");
                    return 1;
        }

        default: 
            break; 
    } 
} 

/* history function to store and print recent history commands 
 * maximum number of commands in history supported: 50
*/

int get_history(char *historyOfCommands[], int pos)
{
        int i = 0;
        int noOfHistoryCmds = 1;

        do {
                if (historyOfCommands[i]) {
                        printf("%4d  %s\n", noOfHistoryCmds, historyOfCommands[i]);
                        noOfHistoryCmds++;
                }

                i = (i + 1) % HISTORY_MEM_SIZE;

        } while (i != pos);

        return 0;
}

/* help command for the command usage details */

void get_help(char *reqCommand) 
{ 
    if(reqCommand == NULL){
        puts("\n"
            "\nGully Shell, version 1.0.2(1)-release (260129)"
            "\nThese shell commands are implemented externally in C Language.  Type `help' to see this list."
            "\nType `help name' to find out more about the function `name'."
            "\nList of Commands supported:"
            "\n>pwd"
            "\n>mv"
            "\n>grep"
            "\n>split\n\n");
    }
    else if(strcoll(reqCommand,"pwd")==0){
        puts("\nCOMMAND: pwd"
            "\n\t\tNAME: pwd - Print the full filename of the current working directory."
            "\n\t\tSYNOPSIS: pwd\n");
    }
    else if(strcoll(reqCommand,"mv")==0){
        puts("\nCOMMAND: mv"
            "\n\t\tNAME: mv - move(rename) files"
            "\n\t\tSYNOPSIS: mv SOURCE DESTINATION"
            "\n\t\t\tmv SOURCE DIRECTORY\n");
    }
    else if(strcoll(reqCommand,"grep")==0){
        puts("\n"
            "\n grep [-i] [-E] pattern [ files ... ]"
            "\n");
    }
    else if(strcoll(reqCommand,"split")==0){
        puts("\n"
            "\n split"
            "\n");
    }
    else{
        puts("unrecognized command by gully shell. Please enter \"help\" to see all supported commands\n");
    }

} 

/* primary loop for read, evaluate and print command execution */

void shell_looper(){

    char    commandLineInput[MAX_CMD_SIZE];
    char    *commandLineArgsPiped[MAX_LIST_SIZE];
    int     execFlag                                =   0;
    int     statusOfExecutedCommand;
    pid_t   childPid                                =   0;
    char    *hist[HISTORY_MEM_SIZE];
    int     currentHistPos                          =   0;
    char    **commandLineArgs                       =   malloc(MAX_CMD_SIZE * sizeof(char *));

    /* clear garbage history values if any */

    for (int i = 0; i < HISTORY_MEM_SIZE; i++)
        hist[i] = NULL;

    /* read, evaluate and print indefinite loop */


    while(1){

        /** my_shell should look exactly like the one in your Linux terminal
         *  user@hostname:path to
         *  current dir.
         *  For example: dipika@system_lab :~/Assignment1$
         */
      
        /* display prompt and take input from the user */

        strcpy(commandLineInput,read_prompt());
         
        /* free history of next index */

        free(hist[currentHistPos]);

        /* copy duplicate of input into history */

        hist[currentHistPos] = strdup(commandLineInput);

        /* enable only 50 possible history and replace old ones if it is full with index */

        currentHistPos = (currentHistPos + 1) % HISTORY_MEM_SIZE;

        /* read the command input entered */

        if (strlen(commandLineInput) != 0) {
            add_history(commandLineInput);
        }
        if(strlen(commandLineInput)==0){
            continue;
        }

        /* dissolve input string into command tokens */

        commandLineArgs = parse_input(commandLineInput);

        /** my_shell should also support the exit command so that the process is not 
        *   killed or forcefully exited and
        *   print a meaningful exit message.
        */

        if (strcoll(commandLineArgs[0], "exit") == 0)
        {
            printf("Got: exit! Exiting Gully Shell..Bye Bye\n");
            fflush(stdout);
            exit(0);
        }
        
        /* my_shell should also remember the recently used commands. Hence implement history command for
         * your shell. History function need not be explicitly implemented, but when using UP and DOWN
         * arrows, the recently used commands must be available.
         * */

        /* get history of Gully Shell */

        if (strcoll(commandLineArgs[0], "history") == 0)
        {
            get_history(hist, currentHistPos);
            continue;
        }
        
        /* get help for Gully Shell */

        if (strcoll(commandLineArgs[0], "help") == 0)
        {
            get_help(commandLineArgs[1]);
            continue;
        }
        
        for(int i = 0; i< MAX_CMD_SIZE; i++){
            if(commandLineArgs[i]==NULL)
                break;
        }

        if(executeCmndHandler(commandLineArgs)!=1){
            printf("%s: command not found\n", commandLineArgs[0]);
            //openHelp();
        };

        free(commandLineArgs);
    }
}

//---------------------------------------------------------------------------------------