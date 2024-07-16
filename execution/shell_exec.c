#include "execution.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


char *builtin_str[] = {
    "cd",
    "help",
    "exit",
    "hist"
};

int (*builtin_cmd[]) (char **) = {
    &shell_cd,
    &shell_help,
    &shell_exit,
    &shell_hist,
};

void close_pipes(int pipe_count, int (*pipes)[2]) {
    for(int i = 0; i < pipe_count; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
}

int shell_exec(char **args, int pipe_count, int (*pipes)[2], int dir[2]) {
    if (args[0] == NULL) {
        return 1;
    }

    for (int i = 0; i < BUILTIN_CMD_SIZ; i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_cmd[i])(args);
        }
    }
    return shell_launch(args, pipe_count, pipes, dir);
}
