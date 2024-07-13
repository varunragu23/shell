#include "parsing.h"
#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 512

char* read_input() {
    int cur_bufsize = BUF_SIZE;
    int ind = 0;
    int c;
    char *input = malloc(sizeof(char) * cur_bufsize);

    printf("shell> ");

    if (!input) {
        printf("allocation error\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        c = getchar();
        if (c == '\n' || c == EOF) {
            input[ind] = '\0';
            return input;
        }
        input[ind] = c;
        ind++;

        if (ind >= cur_bufsize) {
            cur_bufsize += BUF_SIZE;
            input = realloc(input, sizeof(char) * cur_bufsize);
            if (!input) {
                printf("allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}
