#include<stdio.h>           //for command line IO
#include<stdlib.h>          //for exit() and malloc()
#include<unistd.h>          //for fork() and execvp()
#include<string.h>          //for strtok() and strcmp()
#include<sys/wait.h>        //for wait()


int main() {

    //create buffer to store command
    char* command;
    size_t size = 256;
    command = malloc(size);

    while(strcmp(command, "exit") != 0) {                   //loop until user enters exit
        
        //Accounts for user pressing enter continuously
        do {
            printf("MoreShell> ");
            getline(&command, &size, stdin);                //read in the line from the terminal
        } while(strcmp(command, "\n") == 0);                //loop if user presses enter 
        
        command = strtok(command, "\n");                    //remove \n at the end of command
        

        char* commandArr[10];                               //create command array for execvp (only supports 8 arguments for command) (can be made bigger to support more)
        for (int i = 0; i < 10; i++) {                      //populate the array with NULL
            commandArr[i] = NULL;
        }

        char* token = strtok(command, " ");                 //tokenize the command using " "
        int count = 0;                                      //count variable to track the index of the command array
        while(token != NULL) {                              //loop until there are no more tokens left to read
            commandArr[count] = token;                      //assign token to command array
            count++;
            token = strtok(NULL, " ");                      //get the next token
        }

        pid_t pid = fork();                                 //create child process to execute command

        if (pid < 0) {                                      //if pid is negative then the fork failed
            printf("Fork failed.\n");
        }
        else if (pid == 0) {                                        //pid = 0 means child is executing
            //execute command in child process
            int status = execvp(commandArr[0], commandArr);         //execute command using execvp
            if (status == -1 && strcmp(command, "exit") != 0) {     //if execvp returns a negative integer (error) exit and tell using command is invalid (unless command was exit)
                printf("Invalid command\n");
                exit(EXIT_FAILURE);
            }
            exit(EXIT_SUCCESS);                                     //exit from child process after executing command
        }
        else {
            wait(NULL);                                             //wait until child process completes
        }
    }


    return 0;
}