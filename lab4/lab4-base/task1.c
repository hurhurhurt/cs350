#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
	pid_t pid;
	int status;
	pid = fork();
	if (pid == 0){ // child process
		printf("IN CHILD: pid = %d\n", getpid());
		execlp("ls", "-a", "-l");
	}
	else if (pid > 0){ //parent process
		pid = wait(&status);
		printf("IN PARENT: successfully waited child pid = %d\n", pid);
	}
	else{ //failed 
		fprintf(stderr, "Fork failed");
		exit(-1);
	}
	return 0;
}
