# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = gcc
CFLAGS  = -g -Wall -c

# All Targets
all: 1c

# Tool invocations
# Executable "1b" depends on the files:
1c: 1c.o
	@echo 'Building target: 1c'
	$(CC) -m32 1c.o -o 1c
	@echo 'Finished building target: 1c'

# Depends on the source file (c)
1c.o: 1c.c
	$(CC) $(CFLAGS) -m32 -o 1c.o 1c.c 

# Clean the build directory and executable
clean:
	rm -f *.o 1c
