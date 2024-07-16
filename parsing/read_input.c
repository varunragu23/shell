#include "parsing.h"
#include "../shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

#define BUF_SIZE 512

// Function to disable canonical mode and echo
void enable_raw_mode() {
    struct termios raw;
    tcgetattr(STDIN_FILENO, &raw);
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

// Function to restore terminal settings
void disable_raw_mode() {
    struct termios raw;
    tcgetattr(STDIN_FILENO, &raw);
    raw.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

// Function to clear the current input line
void clear_line(int input_len) {
    for (int i = 0; i < input_len; i++) {
        printf("\b \b");
    }
    fflush(stdout);
}

// Function to handle the entire input process
char* read_input() {
    int cur_bufsize = BUF_SIZE;
    int ind = 0;
    int c;
    char *input = malloc(sizeof(char) * cur_bufsize);

    printf("shell> ");
    fflush(stdout);

    if (!input) {
        printf("allocation error\n");
        exit(EXIT_FAILURE);
    }

    enable_raw_mode(); // Enable raw mode to handle input manually

    while (1) {
        c = getchar();

        // Handle Tab key (ASCII code 9)
        if (c == '\t') {
            const char *tab_insert = search_shell_hist(input, ind); // Get the autocomplete suggestion
            int len = strlen(tab_insert);

            // Resize if necessary
            if (len >= cur_bufsize) {
                cur_bufsize = len + 1;
                input = realloc(input, sizeof(char) * cur_bufsize);
                if (!input) {
                    printf("allocation error\n");
                    disable_raw_mode(); // Restore terminal settings before exit
                    exit(EXIT_FAILURE);
                }
            }

            // Clear current input from terminal
            for (int i = 0; i < ind; i++) {
                printf("\b \b");
            }

            // Insert the new suggestion into input buffer
            strcpy(input, tab_insert);

            // Print the new suggestion
            printf("%s", tab_insert);
            fflush(stdout);

            // Update the input length
            ind = len;

            continue;
        }

        // Handle backspace key (ASCII code 127)
        if (c == 127) {
            if (ind > 0) {
                ind--;
                // Shift the rest of the string left
                for (int i = ind; input[i] != '\0'; i++) {
                    input[i] = input[i + 1];
                }
                clear_line(ind + 1);
                printf("%s", input);
                printf(" ");
                // Move the cursor back to the correct position
                for (int i = strlen(input + ind); i >= 0; i--) {
                    printf("\b");
                }
                fflush(stdout);
            }
            continue;
        }

        // Handle arrow keys
        if (c == 27) { // Escape sequence
            if (getchar() == '[') {
                c = getchar();
                if (c == 'D') { // Left arrow
                    if (ind > 0) {
                        ind--;
                        printf("\b");
                        fflush(stdout);
                    }
                } else if (c == 'C') { // Right arrow
                    if (ind < strlen(input)) {
                        printf("%c", input[ind]);
                        ind++;
                        fflush(stdout);
                    }
                }
                continue;
            }
        }

        if (c == '\n' || c == EOF) {
            input[ind] = '\0';
            printf("\n");
            disable_raw_mode(); // Restore terminal settings
            return input;
        }

        input[ind] = c;
        ind++;

        // Print the character to the terminal
        putchar(c);
        fflush(stdout);

        // Resize if necessary
        if (ind >= cur_bufsize) {
            cur_bufsize += BUF_SIZE;
            input = realloc(input, sizeof(char) * cur_bufsize);
            if (!input) {
                printf("allocation error\n");
                disable_raw_mode(); // Restore terminal settings before exit
                exit(EXIT_FAILURE);
            }
        }
    }
}