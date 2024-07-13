#include "execution.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int shell_launch(char **args) {
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0) {
        execvp(args[0], args);
        perror("shell");
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("shell");
    } else {
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}
