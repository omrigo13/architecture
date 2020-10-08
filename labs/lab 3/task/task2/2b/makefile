# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = gcc
CFLAGS  = -g -Wall -c

# All Targets
all: 2b

# Tool invocations
# Executable "2b" depends on the files:
2b: 2b.o
	@echo 'Building target: 2b'
	$(CC) -m32 2b.o -o 2b
	@echo 'Finished building target: 2b'

# Depends on the source file (c)
2b.o: 2b.c
	$(CC) $(CFLAGS) -m32 -o 2b.o 2b.c 

# Clean the build directory and executable
clean:
	rm -f *.o 2b
