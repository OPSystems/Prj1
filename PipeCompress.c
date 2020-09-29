#include<stdio.h>          //for file and command line IO
#include<stdlib.h>         //for exit() and malloc()
#include<unistd.h>         //for pipes and fork()
#include<sys/wait.h>       //for wait()

//function to compress an input string and write to output file
void Compress(char* data, FILE* fOut, int f_size) {
    
    int count = 1;                                        //variable to keep track of the count of consecutive 1's and 0's  
    
    //iterate through the input string
    for (int j = 0; j < f_size; j++) {
        if (data[j] != data[j+1]) {                       //if the current character is different from the next character
            if (count < 16) {                             //if count is less than 16
                for (int k = 0; k < count; k++) {         //print out the character count number of times
                    fprintf(fOut, "%c", data[j]);
                }
            }
            else{                                         //if count is greater than or equal to 16
                if(data[j] == '1') {                      //print out +count+ if the current character is a 1
                    fprintf(fOut, "+%d+", count);
                }
                else {                                    //print out -count- if the current character is a 0
                    fprintf(fOut, "-%d-", count);
                }
            }
            count = 1;                                    //reset count
        }
        else {
            count++;                                      //increment count if current character and next character is the same
        }
    }
}

int main(int argc, char *argv[]) {

    //Exit program if invalid number of arguments are given
    if (argc != 3) {
        printf("Program requires 2 arguments in this order: (1) input file path and (2) output file path.\n");
        exit(1);
    }


    pid_t pid;      //variable to store pid when forking
    int p[2];       //array to store pipe's file descriptors

    //create pipe
    if (pipe(p) == -1) {
        printf("Failed to create the pipe. \n");
    }

    //fork child process and store pid
    pid = fork();


    if (pid < 0) {                                                          //check if fork failed
        printf("Error: fork failed. \n");
    } else if (pid == 0){                                                   //pid = 0 means the process is the child process
        
        FILE* fOut = fopen(argv[2], "w");                                   //file pointer for output file
        
        close(p[1]);                                                        //close the write end of the pipe
        
        char* data;                                                         //buffer to store the file
        long f_size;                                                        //variable to store the size of the file
        
        read(p[0], &f_size, sizeof(f_size));                                //read in the size of the file from the pipe
        data = malloc(f_size);                                              //allocate the size of the file into the buffer
        
        int readLength;                                                     //variable to store the size of the input read from the pipe
        char character;                                                     //variable to store the input read from the pipe
        int i = 0;                                                          //variable to store the current index in the buffer

        while((readLength = read(p[0], &character, 1)) > 0) {               //keep reading from the pipe until the input read is of length 0        
            data[i] = character;                                            //append the input read to the buffer
            i++;
        }
        if (readLength < 0) {                                               //if read returns a negative number (error) then exit
            exit(1);
        }

        //output a compressed version of the file into the output file
        Compress(data, fOut, f_size);                                       //run Compress function on buffer

        fclose(fOut);                                                       //close output file

        close(p[0]);                                                        //close read end of pipe
    } else {

        FILE* fRead = fopen(argv[1], "r");                                  //file pointer for input file

        //Exit program if input file does not exist
        if (fRead == NULL) {
            printf("File %s does not exist.\n", argv[1]);
            exit(1);
        }
    
        close(p[0]);                                                        //close read end of pipe

        fseek(fRead, 0, SEEK_END);                                          //go to end of input file
        long f_size = ftell(fRead);                                         //get position at the end of the input file (size)
        fseek(fRead, 0, SEEK_SET);                                          //return to the beginning of input file
        write(p[1], &f_size, sizeof(f_size));                               //write the size of the file to the pipe

        int character;                                                      //variable to store the characters being read from the file (is an int to compare with EOF)
        while((character = fgetc(fRead)) != EOF) {                          //read until end of file
            char buffer = (char) character;                                 //convert to character
            write(p[1], &buffer, 1);                                        //write each character read to the pipe
        } 
        
        close(p[1]);                                                        //close write end of pipe

        printf("File \"%s\" compressed successfully to \"%s\".\n", argv[1], argv[2]);    //Output confirmation that the program executed successfully
    }

    

    return 0;
}