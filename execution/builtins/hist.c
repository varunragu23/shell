#include "../execution.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cmd_hist_init(int init_size) {
    cmd_hist.args = malloc(sizeof(char*) * init_size);
    if(!cmd_hist.args) {
        fprintf(stderr, "hist allocation error\n");
        exit(EXIT_FAILURE);
    }
    cmd_hist.cur_size = init_size;
    cmd_hist.pos = 0;
    cmd_hist.lst_vis = 0;
}

void add_cmd(char *line) {
    cmd_hist.args[cmd_hist.pos] = strdup(line);
    if(!cmd_hist.args[cmd_hist.pos]) {
        printf("hist dupl error\n");
        exit(EXIT_FAILURE);
    }
    cmd_hist.lst_vis = cmd_hist.pos;
    cmd_hist.pos++;
    if(cmd_hist.pos >= cmd_hist.cur_size) {
        cmd_hist.cur_size += cmd_hist.cur_size;
        cmd_hist.args = realloc(cmd_hist.args, sizeof(char*) * cmd_hist.cur_size);
        if(!cmd_hist.args) {
            printf("hist allocation error\n");
            exit(EXIT_FAILURE);
        }
    }
    
}

void free_cmd_hist() {
    for(int i = 0; i < cmd_hist.cur_size; i++) {
        free(cmd_hist.args[i]);
    }
    free(cmd_hist.args);
}

int shell_hist(char **args) {
    printf("Printing Command History...\n");
    int count = 0;
    for(int i = cmd_hist.pos - 1; i >= 0; i--) {
        count++;
        if(count%2) {
            printf("\033[32m%d. %s\033[0m\n", count, cmd_hist.args[i]);
        }
        else {
            printf("\033[33m%d. %s\033[0m\n", count, cmd_hist.args[i]);
        }
    }

    return 1;
}

char* search_shell_hist(char *input, int len) {
    int search_count = 0;
    for(int i = cmd_hist.lst_vis; i >= 0 && search_count < cmd_hist.pos; i--) {
        search_count++;
        if(strlen(cmd_hist.args[i]) < len) continue;
        if(strncmp(input, cmd_hist.args[i], len) == 0) {
            cmd_hist.lst_vis = i - 1;
            if(cmd_hist.lst_vis < 0) cmd_hist.lst_vis = cmd_hist.pos - 1;
            return cmd_hist.args[i];
        }
    }
    cmd_hist.lst_vis = cmd_hist.pos - 1;
    char *def_string;
    def_string = malloc(sizeof(char) * len);
    if(!def_string) {
        printf("alloc error\n");
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < len; i++) {
        def_string[i] = input[i];
    }
    return def_string;
}

void reset_hist_lst_vis() {
    cmd_hist.lst_vis = cmd_hist.pos - 1;
}
