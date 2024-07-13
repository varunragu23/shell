#include "shell.h"
#include <stdlib.h>

CommandHistory cmd_hist;

void shell_loop() {
    char *input;
    char **args;
   cmd_hist_init(8);
    do {
        input = read_input();
        add_cmd(input);
        args = parse_args(input);
        if (shell_exec(args)) {
            free(input);
            free(args);
            continue;
        } else {
            free(input);
            free(args);
            free_cmd_hist();
            exit(EXIT_SUCCESS);
        }
    } while (1);
}

int main(int argc, char **argv) {
    shell_loop();
    return 0;
}
