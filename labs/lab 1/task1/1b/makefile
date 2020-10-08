# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = gcc
CFLAGS  = -g -Wall -c

# All Targets
all: encoder

# Tool invocations
# Executable "lab1" depends on the files:
encoder: main.o
	@echo 'Building target: encoder'
	$(CC) -m32 main.o -o encoder
	@echo 'Finished building target: encoder'

# Depends on the source file (c)
main.o: main.c
	$(CC) $(CFLAGS) -m32 -o main.o main.c 

# Clean the build directory and executable
clean:
	rm -f *.o encoder
