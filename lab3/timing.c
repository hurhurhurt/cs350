#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/types.h>

// To calculate the difference between two timevals:
//    timeval_subtract(&diff, &end, &start)
// ------
// Subtract the ‘struct timeval’ values X and Y,
// storing the result in RESULT.
// Return 1 if the difference is negative, otherwise 0.
// source: https://www.gnu.org/software/libc/manual/html_node/Elapsed-Time.html
int timeval_subtract (struct timeval *result, struct timeval *x, struct timeval *y)
{
  /* Perform the carry for the later subtraction by updating y. */
  if (x->tv_usec < y->tv_usec) {
    int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
    y->tv_usec -= 1000000 * nsec;
    y->tv_sec += nsec;
  }
  if (x->tv_usec - y->tv_usec > 1000000) {
    int nsec = (x->tv_usec - y->tv_usec) / 1000000;
    y->tv_usec += 1000000 * nsec;
    y->tv_sec -= nsec;
  }

  /* Compute the time remaining to wait.
     tv_usec is certainly positive. */
  result->tv_sec = x->tv_sec - y->tv_sec;
  result->tv_usec = x->tv_usec - y->tv_usec;

  /* Return 1 if result is negative. */
  return x->tv_sec < y->tv_sec;
}

// USE THE FOLLOWING COMMAND TO COMPILE:
//		gcc timer.c -o timer
int main(int argc, char * argv[]) {
	struct timeval start, end, diff;
	pid_t pid;
	int status;
	pid = fork();
	gettimeofday(&start, NULL);
	if (pid == 0){ // child process
		int s = execvp(argv[1], &argv[1]);
		if (s == -1){
			printf("Command %s does not exist", argv);
		}
	}
	else if (pid > 0){ //parent process
		printf("Parent %d created child %d\n", getppid(), getpid());
		pid = wait(&status);
	}
	else{ //failed 
		fprintf(stderr, "Fork failed");
    		exit(-1);
	}
	gettimeofday(&end, NULL);
	timeval_subtract(&diff, &end, &start);
 	printf("Run Time: %ld.%04ld (s)\n", diff.tv_sec, diff.tv_usec/1000);
 	//int execv(const char *pathname, char *const argv[]);
 	return 0;
}
