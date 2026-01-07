# Compilation Variables
CC = gcc
CFLAGS = -Wall -g
LIBS = -lgmp

# Generic Rule to Compile and Link C Programs:
# When we run 'make program_name', this rule is used.
# Make will try to create an executable with the name of the TARGET ($@).
# It assumes there is a corresponding C file ($@.c).
%: %.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

