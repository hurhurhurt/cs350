#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

static volatile int running = 1;

void handlec(){
  running = 0;
}

void printcmd(struct cmd *cmd){
}

void execute(struct cmd *cmd)
{
  struct backcmd *bcmd = NULL;
  struct execcmd *ecmd = NULL;
  struct listcmd *lcmd = NULL;
  struct pipecmd *pcmd = NULL;
  struct redircmd *rcmd = NULL;

  int i = 0;

  if(cmd == NULL)
    {
      PANIC("NULL addr!");
      return;
    }

  switch(cmd->type){
  case EXEC:
    ecmd = (struct execcmd*)cmd;
    pid_t pid;
    int status;
    pid = fork();
    if (pid == 0){ // child process
      int s = execvp(ecmd->argv[0], &ecmd->argv[0]);
    }
    else if (pid > 0){ //parent process
      pid = wait(&status);
      int exit_status = WEXITSTATUS(status);
      if (exit_status !=0){
	printf("Non-zero exit code (%d) detected\n", exit_status);
      }
    }
    else{ //failed
      fprintf(stderr, "Fork failed");
      exit(-1);
    }
    waitpid(-1, NULL, WNOHANG);

    break;

  case REDIR:
    rcmd = (struct redircmd*)cmd;
    ecmd = (struct execcmd*)rcmd->cmd;

    pid = fork();
    if (pid < 0){
      fprintf(stderr, "Error opening file\n");
      exit(0);
    }

    if (pid == 0){
      int fd = open(rcmd->file, rcmd->mode, 0666);
      if (fd < 0){
	fprintf(stderr, "Error opening file\n", rcmd->file);
	exit(0);
      }

      if (rcmd->fd_to_close == 1){
	dup2(fd, 1);
      }
      else {
	dup2(fd, 0);
      }

      if (ecmd->type == REDIR){
	execute((struct cmd *)ecmd);
	exit(0);
      }
      else{
	execvp(ecmd->argv[0], &ecmd->argv[0]);
      }
    }
    else{
      wait(&status);
    }
    break;

  case LIST:
    lcmd = (struct listcmd*)cmd;
    pid = fork();
    if (pid == 0){ // child process
      ecmd = (struct execcmd*)lcmd->left;
      int s = execvp(ecmd->argv[0], &ecmd->argv[0]);
    }
    else{
      pid = fork();
      if (pid == 0){ // child process
	ecmd = (struct execcmd*)lcmd->right;
	int a = execvp(ecmd->argv[0], &ecmd->argv[0]);
      }
      else if (pid > 0){ //parent process
	for (int i = 0; i < 2; i++){
	  pid = wait(&status);
	  int exit_status = WEXITSTATUS(status);
	  if (exit_status !=0){
	    printf("Non-zero exit code (%d) detected\n", exit_status);
	  }
	}
      }
    }

    break;

  case PIPE:
    pcmd = (struct pipecmd*)cmd;
    int fd[2];
    pipe(fd);
    if (fork() == 0){
      ecmd = (struct execcmd*)pcmd->left;
      dup2(fd[1], 1);
      close(fd[0]);
      close(fd[1]);
      execvp(ecmd->argv[0], &ecmd->argv[0]);
    }
    if (fork() == 0){
      dup2(fd[0], 0);
      close(fd[1]);
      close(fd[0]);
      if (pcmd->right->type == PIPE){
	execute(pcmd->right);
	exit(0);
      }
      else{
	ecmd = (struct execcmd*)pcmd->right;
	execvp(ecmd->argv[0], &ecmd->argv[0]);
      }
    }
    close(fd[0]);
    close(fd[1]);
    for (int i = 0; i < MAXARGS - 1; i++){
      wait(&status);
    }
    setbuf(stdout, NULL);
    break;

  case BACK:
    bcmd = (struct backcmd*)cmd;
    pid = fork();
    if (pid < 0){
      fprintf(stderr, "Fork failed");
      exit(-1);
    }
    else if (pid == 0){
      setpgid(0,0);
      execute(bcmd->cmd);
      exit(0);
    }  
    
    break;

  default:
    PANIC("");
  }
 printcmd_exit:

  return;
}




int main(void)
{
  static char buf[1024];
  int fd;

  setbuf(stdout, NULL);
  signal(SIGINT, handlec);
  
  while(getcmd(buf, sizeof(buf)) >= 0)
    {
      if (running == 0){
	printf("Ctrl-C Catched, but there is currently no foreground process running\n");
	running = 1;
      }
      struct cmd * command;
      command = parsecmd(buf);
      execute(command); 
    }
  exit(0);

  PANIC("getcmd error!\n");
  return 0;
}
