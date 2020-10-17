#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char * argv[]) {
    pid_t pid;
	int status;
	pid = fork();
	if (pid == 0){ // child process
		printf("IN CHILD: pid = %d\n", getpid());
		int s = execvp(argv[1], &argv[1]);
	}
	else if (pid > 0){ //parent process
		printf("IN PARENT: successfully waited child pid = %d\n", pid);
		pid = wait(&status);
	}
	else{ //failed 
		fprintf(stderr, "Fork failed");
		exit(-1);
	}
	return 0;
}
