#include "../execution.h"
#include <stdio.h>


extern char *builtin_str[];

int shell_help(char **args) {
    printf("Varun's Shell\n");
    printf("The available commands are:\n");
    for (int i = 0; i < BUILTIN_CMD_SIZ; i++) {
        printf("%d. %s\n", i + 1, builtin_str[i]);
    }
    return 1;
}
