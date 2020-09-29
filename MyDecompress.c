#include<stdio.h>       //for input and output to console and files
#include<stdlib.h>      //for exit()

int main(int argc, char *argv[]) {


    //Exit program if invalid number of arguments are given
    if (argc != 3) {
        printf("Program requires 2 arguments in this order: (1) input file path and (2) output file path.\n");
        exit(1);
    }

    FILE* fRead = fopen(argv[1], "r");  //file pointer to read input file

    //Exit program if input file does not exist
    if (fRead == NULL) {
        printf("File %s does not exist.\n", argv[1]);
        exit(1);
    }

    FILE* fOut = fopen(argv[2], "w");   //file pointer to write to output file


    int character;  //variable to store current character while iterating through the input file

    while((character = fgetc(fRead)) != EOF) {                              //iterate until end of file
        int count = 0;                                                      //count variable to store count if +count+ or -count- is encountered
        if ((char) character == '+' || (char) character == '-') {           //if + or - are encountered 
            fscanf(fRead, "%d", &count);                                    //read in the integer after + or -
            for(int i = 0; i < count; i++) {                                
                if ((char) character == '+') {                              //if + was encountered, output 'count' 1's
                    fprintf(fOut, "1");
                }
                else {                                                      //if - was encountered, output 'count' 0's       
                    fprintf(fOut, "0");
                }
            }
            fgetc(fRead);                                                   //read in the + or - after the number
        }
        else {
            fprintf(fOut, "%c", (char) character);                          //if + or - was not encountered output the character that was read
        }
    }

    fclose(fRead);           //close input file
    fclose(fOut);            //close output file

    printf("File \"%s\" decompressed successfully to \"%s\".\n", argv[1], argv[2]);   //print out that the file ran successfully

    return 0;
}