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
	char * argv3[] = {"wc", "-l", 0};

	int pipe1[2];
	int pipe2[2];
	pipe(pipe1);
	pipe(pipe2);

	pid_t pid, pid2;
	pid = fork();
	if (pid == 0){
		pid2 = fork();	
		if (pid2 == 0){ // CHILD 2
			close(pipe2[1]);
			dup2(pipe2[0], 0);
			close(pipe2[0]);
			close(pipe1[1]);
			close(pipe1[0]);
			execvp(argv3[0], &argv3[0]);
			exit(1);
		}
		else{ // CHILD 1
			close(pipe1[1]);
			dup2(pipe1[0], 0);
			close(pipe1[0]);
			close(pipe2[0]);
			dup2(pipe2[1], 1);
			close(pipe2[1]);
			execvp(argv2[0], &argv2[0]);
			exit(1);
		}
	}
	else{ // PARENT
		close(pipe1[0]);
		dup2(pipe1[1], 1);
		close(pipe1[1]);
		execvp(argv1[0], &argv1[0]);
	}
	setbuf(stdout, NULL);
	return 0;

}
