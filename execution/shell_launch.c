#include "execution.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int pipe_exec(char **args, int pipe_count, int (*pipes)[2], int dir[2]) {
    if (dir[0] != -1) {
        if (dup2(dir[0], STDIN_FILENO) == -1) {
            perror("dup2 for stdin");
            exit(EXIT_FAILURE);
        }
    }

    if (dir[1] != -1) {
        if (dup2(dir[1], STDOUT_FILENO) == -1) {
            perror("dup2 for stdout");
            exit(EXIT_FAILURE);
        }
    }
    close_pipes(pipe_count, pipes);

    return execvp(args[0], args);
}

int shell_launch(char **args, int pipe_count, int (*pipes)[2], int dir[2]) {
  pid_t child_pid = fork();

  if (child_pid) {  /* We are the parent. */
    switch(child_pid) {
      case -1:
        fprintf(stderr, "Oh dear.\n");
        return -1;
      default:
        return child_pid;
    }
  } else {  // We are the child. */
    pipe_exec(args, pipe_count, pipes, dir);
    perror("OH DEAR");
    return 0;
}
}
