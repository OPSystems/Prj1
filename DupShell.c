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
    while(strcmp(command, "exit") != 0) {                                   //loop until user enters exit

        //Accounts for user pressing enter continuously
        do {
            printf("DupShell> ");
            getline(&command, &size, stdin);                                //read in the line from the terminal
        } while(strcmp(command, "\n") == 0);                                //loop if user presses enter
        
        
        command = strtok(command, "\n");                                    //removes the "\n" at the end of the input
        
        char* commandArr[10];                                               //create command array for execvp (only supports 8 arguments for command) (can be made bigger to support more)
        for (int i = 0; i < 10; i++) {                                      //populate the array with NULL
            commandArr[i] = NULL;
        }

        char* secondArr[10];                                                //create array to store second set of commands (after pipe) (also stores up to 8 arguments after the pipe)
        for (int i = 0; i < 10; i++) {                                      //populate the second command array with NULL
            secondArr[i] = NULL;
        }

        int pipeYN = 0;                                                     //boolean value that stores whether or not pipe is part of command

        //parse first part of command (until pipe if pipe exists)
        char* token = strtok(command, " ");                                 //tokenize the command using " "
        int count = 0;                                                      //count variable to track the index of the command array
        while(token != NULL) {                                              //loop until pipe
            if (strcmp(token, "|") == 0) {
                pipeYN = 1;
                token = strtok(NULL, " ");
                break;                                                      //if pipe is encountered break from the loop
            }
            commandArr[count] = token;                                      //store tokens in the command array
            count++;
            token = strtok(NULL, " ");
        }

        //parse second part of command (after pipe)
        count = 0;                                                          //reset count to 0          
        while(token != NULL) {                                              //loop through the rest of the command and store in the second command array
            secondArr[count] = token;
            count++;
            token = strtok(NULL, " ");
        }


        //create pipe to transfer data from first command to second
        int pipefds[2];                                                     //create array to store pipe file descriptors
        if (pipe(pipefds) == -1) {                                          //if pipe returns -1 (error) exit and tell user pipe creation failed
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        pid_t pid = fork();                                                 //Create child process to execute first command

        if (pid < 0) {                                                      //negative pid means fork failed (let user know if fork failed)
            printf("Fork failed.\n");
        }
        else if (pid == 0) {                                                //pid = 0 means child process is executing
            
            if (pipeYN == 1) {                                              //if pipe is in command
                dup2(pipefds[1], STDOUT_FILENO);                            //redirect STDOUT to write end of pipe
            }
            
            int status = execvp(commandArr[0], commandArr);                 //execute first command using execvp
            if (status == -1 && strcmp(command, "exit") != 0) {             //if execvp returns -1 (error) exit child process (unless user enters exit)
                printf("Invalid command\n");
                exit(EXIT_FAILURE);
            }
            close(pipefds[0]);                                              //close read end of pipe
            close(pipefds[1]);                                              //close write end of pipe
        }
        else {                                                              //pid greater than 0 means main process is executing
            if (pipeYN == 1) {                                              //create second child process only if pipe exists in command

                pid_t pid2 = fork();                                        //create child process for second command
                
                if (pid2 < 0) {                                             //negative pid means child process failed to be created
                    printf("Fork failed\n");                                //let user know child process failed to be created
                }
                else if (pid2 == 0) {                                       //pid = 0 means child process is being executed

                    dup2(pipefds[0], STDIN_FILENO);                         //redirect STDIN to read end of the pipe  
                    close(pipefds[1]);                                      //close the write end of the pipe

                    int status = execvp(secondArr[0], secondArr);           //execute the second command using execvp
                    if (status == -1) {                                     //let user know if command is invalid unless user entered exit
                        printf("Invalid command\n");
                        exit(EXIT_FAILURE);
                    }
                    close(pipefds[0]);                                      //close the read end of the pipe
                    
                    exit(EXIT_SUCCESS);                                     //exit child process
                }
                else {
                    wait(NULL);                                             //wait for child process to finish
                }
            }
            close(pipefds[0]);                                              //close the read end of the pipe
            close(pipefds[1]);                                              //close the write end of the pipe
            wait(NULL);                                                     //wait for child process to finish
        }
    }


    return 0;
}