#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


#define BUF_SIZE 512

char* read_input() {

   int cur_bufsize = BUF_SIZE;
   int ind = 0;
   int c;

   char *input = malloc(sizeof(char) * cur_bufsize);

   printf("shell>");

   if(!input) {
      printf("allocation error\n");
      exit(EXIT_FAILURE);
   }

   while(1) {
      // k we need to read in a character
      c = getchar();

      // now we need to add it to input

      // case: we are at the end
      if(c == '\n' || c == EOF) {
         input[ind] = '\0';
         // printf("%d %d\n", ind, cur_bufsize);
         return input;
      }

      // case: normal char addition

      input[ind] = c;

      // update index now that the current ind is used
      ind++;

      // possible error: user has too long input, greater than BUF_SIZE

      if(ind >= cur_bufsize) {
         cur_bufsize += BUF_SIZE;
         input = realloc(input, sizeof(char) * cur_bufsize);
         // check if allocation error again...
         if(!input) {
            printf("allocation error\n");
            exit(EXIT_FAILURE);
         }
      }
   }
}

#define ARG_BUF_SIZE 16

char** parse_args(char *input) {

   int cur_bufsize = ARG_BUF_SIZE;

   char **args = malloc(sizeof(char*) * cur_bufsize);

   int ind = 0;

   if(!args) {
      printf("arg parsing allocation error\n");
      exit(EXIT_FAILURE);
   }

   char *arg;

   while (1) {
      if(ind == 0) {
         arg = strtok(input, " ");
      }
      else {
         arg = strtok(NULL, " ");
      }
      // end of input, processed all args...
      if(arg == NULL) {
         args[ind] = 0;
         return args;
      }

      // what now
      // update args
      args[ind] = arg;
      ind++;
      if(ind >= cur_bufsize) {
         cur_bufsize += ARG_BUF_SIZE;
         args = realloc(args, sizeof(char*) * cur_bufsize);
         if(!args) {
            printf("arg parsing allocation error\n");
            exit(EXIT_FAILURE);
         }
      }
   } 
}


int shell_cd(char **args);
int shell_help(char **args);
int shell_exit(char **args);

#define BUILTIN_CMD_SIZ 3

char *builtin_str[] = {
   "cd",
   "help",
   "exit"
};

int (*builtin_cmd[]) (char **) = {
   &shell_cd,
   &shell_help,
   &shell_exit
};

int shell_cd(char **args) {
   if(args[1] == NULL) {
      fprintf(stderr, "Expected args: \"cd [arg]\"\n");
   }
   else {
      if(chdir(args[1]) != 0) {
         perror("cd");
      }
   }

   return 1;
}

int shell_help(char **args) {
   printf("Varun's Shell\n");
   printf("The available commands are:\n");
   for(int i = 0; i < BUILTIN_CMD_SIZ; i++) {
      printf("%d. %s\n", i + 1, builtin_str[i]);
   }
   return 1;
}

int shell_exit(char **args) {
   printf("Exiting...\n");
   return 0;
}

int shell_launch(char **args) {
   pid_t pid;
   int status;

   pid = fork();
   if(pid == 0) {
      // child
      execvp(args[0], args);
      perror("shell");
      exit(EXIT_FAILURE);
   }
   else if(pid < 0) {
      // kill him
      perror("shell");
   }
   else {
      // parent
      do {
         waitpid(pid, &status, WUNTRACED);
      } while(!WIFEXITED(status) && !WIFSIGNALED(status));
   }

   return 1;
}

int shell_exec(char **args) {
   if(args[0] == NULL) {
      printf("there\n");
      return 1;
   }

   for(int i = 0; i < BUILTIN_CMD_SIZ; i++) {
      if(strcmp(args[0], builtin_str[i]) == 0) {
         return (*builtin_cmd[i])(args);
      }
   }
   return shell_launch(args);
}

void shell_loop() {

   // we have input from the user

   char *input;

   // we have to parse the user's arguments

   char **args;

   // execute their command...

   do {
      input = read_input();
      args = parse_args(input);
      if(shell_exec(args)) {
         free(input);
         free(args);
         continue;
      }
      else {
         free(input);
         free(args);
         exit(EXIT_SUCCESS);
      }
   } while(1);

}

int main(int argc, char **argv) {
   shell_loop();

   return 0;
}