#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>

void handler(int sig)
{
    FILE *out=fopen("logs.txt","a");
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    fputs(asctime(timeinfo),out);
    printf("  The child process terminated without errors.\n");
    fputs("\nThe child process terminated without errors.\n",out);   
   fclose(out); 
}

int main()
{	
    //child data structures
    pid_t child_pid, wpid;
    int status = 0;
    char* argv[20];

    //Parsing commands
    char* token;
    char command[100];

    while(1){
    printf( "$: ");
    fgets(command,20,stdin);
    char last_char = command[strlen(command)-2];
    if(last_char==38){command[strlen(command)-3] = '\0';}
    else{ command[strlen(command)-1] = '\0'; }
    printf("%s\n",command);


    //grouping command arguments into argv array
    token = strtok(command, " ");
    int i=0;
    while( token!=NULL )
    {
    argv[i] = malloc(strlen(token)+1);
    strncpy(argv[i],token,strlen(token));
    i++;
    token = strtok(NULL, " ");
    }
    //append Null value to the end of the arguments array
    argv[i] = NULL;

    signal(SIGCHLD, handler);

    // fork/spawn new process.
    child_pid = fork();
    //if correctly spawned->execute command
    if ((child_pid== 0))
    {
        FILE *out=fopen("logs.txt","a");
        fputs("command: ",out);
        fputs(command,out);
        fputs("\n time:\n",out);
        fclose(out);
        printf("%s",argv[1]);
        status = execvp(command, argv);
        exit(0);
    }
    else if(child_pid<0)
    {
        printf("Error");
    }

    else
    {
        int returnStatus; 
        if(last_char!='&')
        {
            // Parent process waits here for child to terminate.
            waitpid(child_pid, &returnStatus, 0);  
        }

        if (returnStatus == 1)      
        {
           printf("An error occured!.");   
        }
      }
}
    return 0;
}
