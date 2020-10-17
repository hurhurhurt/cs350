#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define INPUT_FILE "./input/if1"

int main(int argc, char * argv[]) {
    pid_t pid;
	int status;
	pid = fork();
	if (pid == 0){ // child process
		printf("IN CHILD: pid = %d\n", getpid());
		int fd = open(INPUT_FILE, O_RDONLY);
		int fd_out = open("./result.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
		dup2(fd, 0);
		dup2(fd_out, 1);
		int s = execvp(argv[1], &argv[1]);
		close(fd);
		close(fd_out);
		
		
		
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
    return 0;
}
