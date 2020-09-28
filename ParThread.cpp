#include <stdio.h>
#include <pthread.h>

void *compress(void *) {
	int sum = 0;
    for(int i = 0; i < 100000; i++)
        sum += i;
    printf("Hi, this is pthread!\n");
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {

	pthread_t thread_id[10];

	for(int i = 0; i < 10; i++) {
	    pthread_create(&thread_id[i], NULL, compress, NULL);
		
	}

	for(int i = 0; i < 10; i++)
	    pthread_join(thread_id[i], NULL);

	return 0;
}