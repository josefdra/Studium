#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

int globl = 1;
int *globl_ptr = &globl;

int main()
{
pid_t pid;

	*globl_ptr += 2;

	//fork a child process
	pid = fork();

	if(pid < 0){ //error occurred
		fprintf(stderr, "Fork Failed\n");
		return 1;
	}
	else if(pid == 0){ //child process
		printf("Reading globl from Child: %d\n", *globl_ptr);
		*globl_ptr = 3;
		}
	else{ //parent process
		//parents will wait for the child to complete
		wait(NULL);
		printf("Child Complete\n");
		printf("Reading globl from Parent: %d\n", *globl_ptr);
		*globl_ptr = 4;
	}
	printf("%d, Finished\n", *globl_ptr);

	return 0;
}	

//Communication between child and parent is not possible due to them not running at the same time
//The execution of the child/parent-process is deterministic
