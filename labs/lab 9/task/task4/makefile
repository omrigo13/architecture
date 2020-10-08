# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = gcc
CFLAGS  = -g -Wall -c

# All Targets
all: virusDetector

# Tool invocations
# Executable "virusDetector" depends on the files:
virusDetector: task4.o
	@echo 'Building target: virusDetector'
	$(CC) -m32 task4.o -o virusDetector
	@echo 'Finished building target: virusDetector'

# Depends on the source file (c)
task4.o: task4.c
	$(CC) $(CFLAGS) -m32 -o task4.o task4.c 

# Clean the build directory and executable
clean:
	rm -f *.o virusDetector
