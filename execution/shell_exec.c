#include "execution.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


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

int shell_exec(char **args) {
    if (args[0] == NULL) {
        return 1;
    }

    for (int i = 0; i < BUILTIN_CMD_SIZ; i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_cmd[i])(args);
        }
    }
    return shell_launch(args);
}
