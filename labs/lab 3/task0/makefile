# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = gcc
CFLAGS  = -g -Wall -c

# All Targets
all: hexaPrint

# Tool invocations
# Executable "lab3_bubbleSort" depends on the files:
hexaPrint: hexaPrint.o
	@echo 'Building target: hexaPrint'
	$(CC) -m32 hexaPrint.o -o hexaPrint
	@echo 'Finished building target: hexaPrint'

# Depends on the source file (c)
hexaPrint.o: hexaPrint.c
	$(CC) $(CFLAGS) -m32 -o hexaPrint.o hexaPrint.c 

# Clean the build directory and executable
clean:
	rm -f *.o hexaPrint
