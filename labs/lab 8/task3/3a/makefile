# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = gcc
CFLAGS  = -g -Wall -c

# All Targets
all: myELF

# Tool invocations
# Executable "myELF" depends on the files:
myELF: task3a.o
	@echo 'Building target: myELF'
	$(CC) -m32 task3a.o -o myELF
	@echo 'Finished building target: myELF'

# Depends on the source file (c)
task3a.o: task3a.c
	$(CC) $(CFLAGS) -m32 -o task3a.o task3a.c 

# Clean the build directory and executable
clean:
	rm -f *.o myELF
