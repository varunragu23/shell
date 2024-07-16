#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

CommandHistory cmd_hist;




void shell_loop() {
    char *input;
    // char **args;
   cmd_hist_init(8);
    do {
        input = read_input();
        add_cmd(input);

        pipelines* cur_pipeline = parse_pipelines(input);

        int pipe_count = cur_pipeline->pipe_count - 1;
        

        int (*pipes)[2] = calloc(pipe_count, sizeof(int[2]));


        for(int i = 1; i <= pipe_count; i++) {
            pipe(pipes[i - 1]);
            cur_pipeline->dir[i][STDIN_FILENO] = pipes[i - 1][0];
            cur_pipeline->dir[i - 1][STDOUT_FILENO] = pipes[i - 1][1];
        }


        if(pipe_count == 0) {
            int ret = shell_exec(cur_pipeline->cmds[0], pipe_count, pipes, cur_pipeline->dir[0]);
            if(ret == 0) {
                free(input);
                free_cmd_hist();
                exit(EXIT_SUCCESS);
            }
            close_pipes(pipe_count, pipes);
            wait(NULL);
            continue;
        }

        for(int i = 0; i <= pipe_count; i++) {
            shell_exec(cur_pipeline->cmds[i], pipe_count, pipes, cur_pipeline->dir[i]);
        }

        close_pipes(pipe_count, pipes);

        for(int i = 0; i < pipe_count + 1; i++) {
            wait(NULL);
        }

    } while (1);
}

int main(int argc, char **argv) {
    shell_loop();
    return 0;
}
