#include "parsing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARG_BUF_SIZE 16

char** parse_args(char *input) {
    int cur_bufsize = ARG_BUF_SIZE;
    char **args = malloc(sizeof(char*) * cur_bufsize);
    int ind = 0;

    if (!args) {
        printf("arg parsing allocation error\n");
        exit(EXIT_FAILURE);
    }

    char *arg;
    while (1) {
        if (ind == 0) {
            arg = strtok(input, " ");
        } else {
            arg = strtok(NULL, " ");
        }
        if (arg == NULL) {
            args[ind] = 0;
            return args;
        }

        args[ind] = arg;
        ind++;
        if (ind >= cur_bufsize) {
            cur_bufsize += ARG_BUF_SIZE;
            args = realloc(args, sizeof(char*) * cur_bufsize);
            if (!args) {
                printf("arg parsing allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    } 
}
