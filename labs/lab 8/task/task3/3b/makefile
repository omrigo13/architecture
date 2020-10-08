# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = gcc
CFLAGS  = -g -Wall -c

# All Targets
all: myELF

# Tool invocations
# Executable "myELF" depends on the files:
myELF: task3b.o
	@echo 'Building target: myELF'
	$(CC) -m32 task3b.o -o myELF
	@echo 'Finished building target: myELF'

# Depends on the source file (c)
task3b.o: task3b.c
	$(CC) $(CFLAGS) -m32 -o task3b.o task3b.c 

# Clean the build directory and executable
clean:
	rm -f *.o myELF
