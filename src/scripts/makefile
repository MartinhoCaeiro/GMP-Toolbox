# Compilation Variables
CC = gcc
CFLAGS = -Wall -g
LIBS = -lgmp

# Automatically detects all .c files and creates a list of executables
SOURCES = $(wildcard *.c)
EXECUTABLES = $(SOURCES:.c=)

# Default target: compiles all executables that do not exist or are outdated
.PHONY: all clean

all: $(EXECUTABLES)

# Generic rule to compile and link each program:
# When we do 'make program_name', this rule is used.
# Make will try to create an executable with the target name ($@).
# It assumes there is a corresponding C file ($@.c).
%: %.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# Cleans up all executables
clean:
	rm -f $(EXECUTABLES)

