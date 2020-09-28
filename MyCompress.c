//gcc -o MyCompress MyCompress.c
//./MyCompress
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "q1.c"

char* myCompression(char *);

int main(){
  char *fromFile = fileReader("source.txt");
  size = sizeString();
  char * compressed = myCompression(fromFile);

  return 0;
}

char * myCompression(char fromFile[]) { 
  int count = 1; 
  char toFile[size];

  for (int i = 0 ; i < size; i++) {
    if(fromFile[i] == fromFile[i+1]){
      count++;
    }
    else{
      if(count < 16){
        // print them out directly
        char temp = fromFile[i];
        printf("%c", fromFile[i]);
        strcat(toFile, temp);
      }
      else{
        // print out the compressed format
        count = 1;
      }
    }
    toFile[i] = '\0';
  }
  //printf("%s",toFile);
  return(fromFile);
}