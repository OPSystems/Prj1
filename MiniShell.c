#include<stdio.h>               //for command line IO
#include<stdlib.h>              //for exit() and malloc()            
#include<unistd.h>              //for fork() and execvp()
#include<string.h>              //for strtok() and strcmp()
#include<sys/wait.h>            //for wait()


int main() {

    //create buffer to store command
    char* command;
    size_t size = 256;
    command = malloc(size);

    while(strcmp(command, "exit") != 0) {                   //loop until user enters exit
        
        //accounts for user pressing enter continuously
        do {
            printf("MiniShell> ");
            fgets(command, size, stdin);                    //read in the line from the terminal
        } while(strcmp(command, "\n") == 0);                //loop if use presses enter

        command = strtok(command, "\n");                    //removes the "\n" at the end of the input

        command = strtok(command, " ");                     //set command to the first word in the line
        

        char* commandArr[2];                                //create command array for execvp (length 2 since execvp requires NULL at the end of the array)
        for (int i = 0; i < 2; i++) {                       //populate the array with NULL
            commandArr[i] = NULL;
        }

        commandArr[0] = command;                            //store command in first index 

        pid_t pid = fork();                                 //create child process to run command

        if (pid < 0) {                                      //if pid is negative fork failed
            printf("Fork failed.\n");
        }
        else if (pid == 0) {                                            //pid = 0 means child is executing
            
            int status = execvp(commandArr[0], commandArr);             //run command using execvp
            if (status == -1 && strcmp(command, "exit") != 0) {         //if execvp returns -1 (error) tell user command is invalid (unless exit was entered)
                printf("Invalid command\n");
                exit(EXIT_FAILURE);
            }
        }
        else {
            wait(NULL);                                     //wait for child process to finish
        }
    }


    return 0;
}