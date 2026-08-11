# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Executable name
TARGET = lexical_analyser

# Source files and Object files
SRCS = main.c lexer.c
OBJS = $(SRCS:.c=.o)

# Default rule: build the executable
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile .c files into .o files
%.o: %.c lexer.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule to remove generated binaries and object files
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets to prevent conflicts with files named 'all' or 'clean'
.PHONY: all clean