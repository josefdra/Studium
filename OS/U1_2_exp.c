#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *func();

void create_threads(pthread_t *ids){
	for(int i=0; i<5; i++){
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&(ids[i]), &attr, func, NULL);
	}
}

void join_threads(pthread_t *ids){
	for(int i=0; i<5; i++)
		pthread_join(ids[i], NULL);
}

int main()
{
	pthread_t tids[5];
	create_threads(tids);
	join_threads(tids);
}

void *func()
{
	for(int i=0; i<5; i++)
		printf("%d\n", i);
	pthread_exit(0);
}
