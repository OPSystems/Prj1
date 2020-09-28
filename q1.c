#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int size;

char * fileReader (char fileName[]) {
	FILE * fileR;
	fileR = fopen (fileName, "r");

	if (!fileR) { 
		printf("ERROR: FILE DOES NOT EXIST");
		return fileName;
	}
	// int size = 0;
	char cur = fgetc(fileR);
	while(cur != EOF){
		size++;
		cur=fgetc(fileR);
	}
  	rewind(fileR);

	size++;
	char *fileTemp = malloc(size);

	int i = 0;
	cur = fgetc(fileR);
	while(cur != EOF){
		fileTemp[i] = cur;
		i++;
		cur = fgetc(fileR);
	}
	fileTemp[i] = '\0';	//� character need to change to the end null https://stackoverflow.com/questions/51523477/array-showing-random-characters-at-the-end

	fclose(fileR);

	return(fileTemp);
}

int sizeString(){
	return size;
}
