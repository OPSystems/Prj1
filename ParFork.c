#include<stdio.h>          //for file and command line IO
#include<stdlib.h>         //for exit() malloc() and atoi()
#include<unistd.h>         //for pipes and fork()
#include<sys/wait.h>       //wait() function for processes to finish
#include<sys/time.h>       //used to time the program

// Function to get integer length (in order to convert it to a string) 
// along with the rest of the characters)
int getIntegerLength(int count) {
    int length = 1;
    while(count/10 != 0) {
        count = count/10;
        length++;
    }
    return length;
}

//Compression function
//Takes in an input string and an output buffer
//Puts the compressed string into the output buffer
//Returns the length of the output string
long Compress(char* data, int f_size, char* output) {
    
    int count = 1;   //count variable to keep track of the number of consecutive 1's and 0's
    
    long length = 0; //length variable to keep track of output string's length (also acts as the current index of the output string)
    for (int j = 0; j < f_size; j++) {
        //check if current char and next char are the same
        if (data[j] != data[j+1]) {
            //if they are not the same check if count is less than 16
            if (count < 16) {
                //if count is less than 16 add the character to the output string 'count' times
                for (int k = 0; k < count; k++) {
                    output[length] = data[j];
                    //increment length
                    length++;
                }
            }
            else{
                //if count is greater than or equal to 16, output compressed format

                //convert integer to string
                char number[getIntegerLength(count)];
                sprintf(number, "%d", count);

                //use + if the character is a 1 and - if the character is a 0
                if(data[j] == '1') {
                    output[length] = '+';
                    length++;
                    for (int i = 0; i < sizeof(number); i++) {
                        output[length] = number[i];
                        length++;
                    }
                    output[length] = '+';
                    length++;
                }
                else {
                    output[length] = '-';
                    length++;
                    for (int i = 0; i < sizeof(number); i++) {
                        output[length] = number[i];
                        length++;
                    }
                    output[length] = '-';
                    length++;
                }
            }
            //reset count to 1
            count = 1;
        }
        else {
            //increment count if they are the same
            count++;
        }
    }
    return length;
}


int main(int argc, char *argv[]) {

    //struct for timing
    struct timeval stop, start;
    gettimeofday(&start, NULL);         //get time at the beginning of program execution

    //Exit program if invalid number of arguments are given
    if (argc != 4) {
        printf("Program requires 3 arguments in this order: (1) input file path, (2) output file path, and (3) number of processes.\n");
        exit(1);
    }

    //file pointer to read file
    FILE* fRead = fopen(argv[1], "r");

    //Exit program if input file does not exist
    if (fRead == NULL) {
        printf("File %s does not exist.\n", argv[1]);
        exit(1);
    }

    //file pointer to write file
    FILE* fOut = fopen(argv[2], "w");

    //number of processes to fork (from user input)
    int n = atoi(argv[3]);

    //create an array of strings to store the contents of the file
    //each element of the array is going to be used by one child process
    char* data[n];
    //array to store the size of each chunk of input (this is created mostly for the last chunk which slightly differs in size)
    long sizes[n];

    //find the size of the file
    fseek(fRead, 0, SEEK_END);
    long f_size = ftell(fRead);
    fseek(fRead, 0, SEEK_SET);
    
    //calculate the chunk size (add 1 for the case that f_size/n = 0)
    long chunk_size = (long) (f_size/n) + 1;

    //allocate the calculated chunk size for each string in the data array
    for (int i = 0; i < n; i++) {
        data[i] = malloc(chunk_size);
    }

    //variable to store the character read from fgetc (in int format so it can be compared with EOF)
    int character;
    //variable to keep track of current chunk of data
    int i = 0;
    //variable to kep track of current character in chunk
    int k = 0;
    while((character = fgetc(fRead)) != EOF) {
        data[i][k++] = (char) character;
        //reset k to 0 and increase i if chunk size is reached
        if (k == chunk_size) {
            //assign k to the sizes array
            sizes[i] = k;
            k = 0;
            i++;
        }
    }
    //assign k to the sizes array 
    sizes[i] = k;

    //close the read file
    fclose(fRead);

    //create a pipe for every child process
    int p[n][2];
    for (int i = 0; i < n; i++) {
        if (pipe(p[i]) == -1) {
            printf("Failed to create the pipe. \n");
        }
    }

    //output a compressed version of the file into the output file
    //fork n child processes
    pid_t pids[n];
    for (int j = 0; j < n; j++) {
        pids[j] = fork();
        //J'TH PROCESS
        if (pids[j] == 0) {
            //allocate memory to store output
            char* outputString = malloc(chunk_size);
            //compress the data in the jth chunk and store in outputString, get output size from the compress function
            long outputSize = Compress(data[j], sizes[j], outputString);
            //close read end of jth pipe
            close(p[j][0]);
            //iterate through the output string and write every character to the jth pipe
            for(long y = 0; y < outputSize; y++) {
                write(p[j][1], &outputString[y], 1);
                //printf("%ld", y);
            }
            //close the write end of the pipe
            close(p[j][1]);
            //exit the child process
            exit(EXIT_SUCCESS);            
        }
    }    


    //wait till all child processes are finished
    int status;
    pid_t pid;

    int m = n;
    while(m > 0) {
        pid = wait(NULL);
        m--;
    }
    
    //iterate through each pipe
    for (int z = 0; z < n; z++) {
        char character;
        //close the write end of the pipe
        close(p[z][1]);
        //read characters from the pipe and write to the output file
        int n;
        while((n = read(p[z][0], &character, 1)) > 0) {
            fprintf(fOut, "%c", character);
        }
        //close read end of pipe
        close(p[z][0]);
    }

    //close output file
    fclose(fOut);
    printf("File \"%s\" compressed successfully to \"%s\".\n", argv[1], argv[2]);    //Output confirmation that the program executed successfully
    
    gettimeofday(&stop, NULL);                                       //get time at the end of file execution
    long seconds = stop.tv_sec - start.tv_sec;                       //get time taken in seconds
    long microseconds = stop.tv_usec - start.tv_usec;                //get time taken in microseconds
    double milliseconds = (seconds + microseconds*1e-6)*1000;        //convert to milliseconds
    printf("The program took: %f milliseconds.\n", milliseconds);     //print the time taken 



    return 0;
}

