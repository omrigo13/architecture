# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = gcc
CFLAGS  = -g -Wall -c

# All Targets
all: 1b

# Tool invocations
# Executable "1b" depends on the files:
1b: 1b.o
	@echo 'Building target: 1b'
	$(CC) -m32 1b.o -o 1b
	@echo 'Finished building target: 1b'

# Depends on the source file (c)
1b.o: 1b.c
	$(CC) $(CFLAGS) -m32 -o 1b.o 1b.c 

# Clean the build directory and executable
clean:
	rm -f *.o 1b
