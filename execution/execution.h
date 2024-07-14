#ifndef EXECUTION_H
#define EXECUTION_H

typedef struct {
    char **args;
    int pos;
    int cur_size;
    int lst_vis;
} CommandHistory;

extern CommandHistory cmd_hist;

#define BUILTIN_CMD_SIZ 4


void cmd_hist_init(int init_size);
void add_cmd(char *line);
void free_cmd_hist();
void reset_hist_lst_vis();

char* search_shell_hist(char *input, int len);

int shell_cd(char **args);
int shell_help(char **args);
int shell_exit(char **args);
int shell_hist(char **args);

int shell_launch(char **args);
int shell_exec(char **args);

#endif // EXECUTION_H
