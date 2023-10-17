#include <pthread.h>
#include <stdio.h>

int shared = 1;

void *thread_function(void *arg) {
    int thread_num = *((int *)arg);
	for(int i=0; i<10; i++){
		int var = shared;
		while(var==shared)
			;
		shared++;		
    	printf("Thread %d changed var to %d\n", thread_num, shared);
	}
}

int main() {
    pthread_t threads[5];
    int thread_args[5];  

    for (int i = 0; i < 5; i++) {
        thread_args[i] = i;
        int result = pthread_create(&threads[i], NULL, thread_function, &thread_args[i]);

        if (result != 0) {
            perror("Thread creation failed");
            return 1;
        }
    }

	shared = 0;

    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("All threads have finished.\n");

    return 0;
}

//Exchange over global variables is possible
//Execution of the threads is non deterministic
