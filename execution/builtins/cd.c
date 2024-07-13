#include "../execution.h"
#include <unistd.h>
#include <stdio.h>

int shell_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "Expected args: \"cd [arg]\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("cd");
        }
    }
    return 1;
}
