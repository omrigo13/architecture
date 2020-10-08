# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = gcc
CFLAGS  = -g -Wall -c

# All Targets
all: hexeditplus

# Tool invocations
# Executable "hexeditplus" depends on the files:
hexeditplus: 1e.o
	@echo 'Building target: hexeditplus'
	$(CC) -m32 1e.o -o hexeditplus
	@echo 'Finished building target: hexeditplus'

# Depends on the source file (c)
1e.o: 1e.c
	$(CC) $(CFLAGS) -m32 -o 1e.o 1e.c 

# Clean the build directory and executable
clean:
	rm -f *.o hexeditplus
