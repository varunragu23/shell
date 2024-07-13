# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# Directories
PARSINGDIR = parsing
EXECUTIONDIR = execution
BUILTINSDIR = $(EXECUTIONDIR)/builtins

# Source files
SRCS = main.c \
       $(PARSINGDIR)/read_input.c \
       $(PARSINGDIR)/parse_args.c \
       $(EXECUTIONDIR)/shell_exec.c \
       $(EXECUTIONDIR)/shell_launch.c \
       $(BUILTINSDIR)/cd.c \
       $(BUILTINSDIR)/help.c \
       $(BUILTINSDIR)/exit.c \
       $(BUILTINSDIR)/hist.c

# Object files
OBJS = $(SRCS:.c=.o)

# Target executable
TARGET = shell

# Default target
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)
