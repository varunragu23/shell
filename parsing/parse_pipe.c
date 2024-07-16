#include "parsing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

pipelines* parse_pipelines(char *input) {
    char* input_cop = strdup(input);
    if (!input_cop) {
        fprintf(stderr, "strdup failed\n");
        exit(EXIT_FAILURE);
    }

    int cmd_cnt = 1;
    for (int i = 0; i < strlen(input_cop); i++) {
        if (input_cop[i] == '|') cmd_cnt++;
    }

    // Allocate memory for the pipelines struct
    pipelines* cur_pipe = malloc(sizeof(pipelines));
    if (!cur_pipe) {
        fprintf(stderr, "malloc failed\n");
        exit(EXIT_FAILURE);
    }

    cur_pipe->pipe_count = cmd_cnt;

    // Allocate memory for the cmds array
    cur_pipe->cmds = malloc(sizeof(char**) * cmd_cnt);
    if (!cur_pipe->cmds) {
        fprintf(stderr, "malloc failed\n");
        free(cur_pipe);
        exit(EXIT_FAILURE);
    }

    // Allocate memory for the pipes array
    cur_pipe->dir = malloc(sizeof(int[2]) * (cmd_cnt));
    if (!cur_pipe->dir) {
        fprintf(stderr, "malloc failed\n");
        free(cur_pipe->cmds);
        free(cur_pipe);
        exit(EXIT_FAILURE);
    }

    char* cur_cmd = strsep(&input_cop, "|");
    int ind = 0;
    while (cur_cmd) {
        cur_pipe->dir[ind][0] = cur_pipe->dir[ind][1] = -1;
        cur_pipe->cmds[ind++] = parse_args(cur_cmd);  // Use strdup to copy command string
        cur_cmd = strsep(&input_cop, "|");
    }

    free(input_cop);
    return cur_pipe;
}
