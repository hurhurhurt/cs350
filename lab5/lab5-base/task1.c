#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    char * argv1[] = {"cat", "Makefile", 0};
    char * argv2[] = {"head", "-4", 0};

	int fd1[2];
	pipe(fd1);

	pid_t pid;
	pid = fork();
	
	if (pid == 0){ // CHILD 1
		printf("IN CHILD-1 (PID = %d): executing command cat ...", getpid());
		dup2(fd1[1], 1);
		close(fd1[0]);
		close(fd1[1]);
		execvp(argv1[0], &argv1[0]);
		exit(1);
	}
	else{
		pid = fork();
		if (pid == 0){ // CHILD 2;
			printf("IN CHILD-2 (PID = %d): executing command head ...", getpid());
			dup2(fd1[0], 0);
			close(fd1[1]);
			close(fd1[0]);
			execvp(argv2[0], &argv2[0]);
			exit(1);
		}
		else{ // PARENT
			int status;
			close(fd1[0]);
			close(fd1[1]);
			wait(&status);
		}
	}
	setbuf(stdout, NULL);
	return 0;
}
	
