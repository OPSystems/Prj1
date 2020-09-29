#include<stdio.h>        //for input and output to console and files
#include<stdlib.h>       //for exit()
#include<sys/wait.h>     //for wait()
#include<unistd.h>       //for fork() and execl()

int main(int argc, char *argv[]) {

    //Exit program if invalid number of arguments are given
    if (argc != 3) {
        printf("Program requires 2 arguments in this order: (1) input file path and (2) output file path.\n");
        exit(1);
    }

    pid_t pid;        //variable to store process id
    pid = fork();     //fork child process and store pid

    if (pid < 0) {                                                      //check if fork failed
        printf("Fork Failed");
    } else if (pid == 0) {                                              //pid = 0 means child process is executing
        execl("MyCompress", "./MyCompress", argv[1], argv[2], NULL);    //use execl to execute MyCompress with the arguments given to this program
    } else {                                                            //pid != 0 means main process is executing
        wait(NULL);                                                     //wait until child completes
        printf("ForkCompress successfully executed MyCompress in child process.\n");    //print out that program successfully executed                                        
    }

    return 0;
}