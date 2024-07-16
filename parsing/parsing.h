#ifndef PARSING_H
#define PARSING_H

typedef struct {
    int pipe_count;
    char*** cmds;
    int (*dir)[2];
} pipelines;

char* read_input();
char** parse_args(char *input);
pipelines* parse_pipelines(char *input);

#endif // PARSING_H
