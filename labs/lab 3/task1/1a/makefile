# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = gcc
CFLAGS  = -g -Wall -c

# All Targets
all: 1a

# Tool invocations
# Executable "1a" depends on the files:
1a: 1a.o
	@echo 'Building target: 1a'
	$(CC) -m32 1a.o -o 1a
	@echo 'Finished building target: 1a'

# Depends on the source file (c)
1a.o: 1a.c
	$(CC) $(CFLAGS) -m32 -o 1a.o 1a.c 

# Clean the build directory and executable
clean:
	rm -f *.o 1a
