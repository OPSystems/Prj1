#include<stdio.h>      //used to read and write to console and files
#include<stdlib.h>     //for malloc() and exit()
#include<sys/time.h>   //used to time the program

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
    
    //struct for timing
    struct timeval stop, start;
    gettimeofday(&start, NULL);         //get time at the beginning of program execution

    //Exit program if invalid number of arguments are given
    if (argc != 3) {
        printf("Program requires 2 arguments in this order: (1) input file path and (2) output file path.\n");
        exit(1);
    }

    FILE* fRead = fopen(argv[1], "r");  //file pointer to read in input
    
    //Exit program if input file does not exist
    if (fRead == NULL) {
        printf("File %s does not exist.\n", argv[1]);
        exit(1);
    }

    FILE* fOut = fopen(argv[2], "w");   //file pointer to write output

    //create string to store the contents of the file
    char* data;

    fseek(fRead, 0, SEEK_END);        //go to end of file
    long f_size = ftell(fRead);       //get location at end of file
    fseek(fRead, 0, SEEK_SET);        //reset location to the beginning of the file
    data = malloc(f_size);            //allocate the size of the file to the data string


    int character;      //variable to store characters being read (type is integer so it can be compared with EOF)
    int i = 0;          //variable to store the current index of the data string

    while((character = fgetc(fRead)) != EOF) {    //iterate until end of file and store in the data string
        data[i++] = (char) character;
    }

    fclose(fRead);   //close the input file

    //output a compressed version of the file into the output file    
    Compress(data, fOut, f_size);

    fclose(fOut);    //close the output file


    printf("File \"%s\" compressed successfully to \"%s\".\n", argv[1], argv[2]);    //Output confirmation that the program executed successfully

    
    gettimeofday(&stop, NULL);                                       //get time at the end of file execution
    long seconds = stop.tv_sec - start.tv_sec;                       //get time taken in seconds
    long microseconds = stop.tv_usec - start.tv_usec;                //get time taken in microseconds
    double milliseconds = (seconds + microseconds*1e-6)*1000;        //convert to milliseconds
    printf("The program took: %f milliseconds.\n", milliseconds);     //print the time taken 

    return 0;
}

