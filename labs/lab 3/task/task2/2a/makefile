# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = gcc
CFLAGS  = -g -Wall -c

# All Targets
all: 2a

# Tool invocations
# Executable "2a" depends on the files:
2a: 2a.o
	@echo 'Building target: 2a'
	$(CC) -m32 2a.o -o 2a
	@echo 'Finished building target: 2a'

# Depends on the source file (c)
2a.o: 2a.c
	$(CC) $(CFLAGS) -m32 -o 2a.o 2a.c 

# Clean the build directory and executable
clean:
	rm -f *.o 2a
