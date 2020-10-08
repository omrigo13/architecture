# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = gcc
CFLAGS  = -g -Wall -c

# All Targets
all: digitCounter

# Tool invocations
# Executable "digitCounter" depends on the files:
digitCounter: task4.o
	@echo 'Building target: digitCounter'
	$(CC) -m32 task4.o -o digitCounter
	@echo 'Finished building target: digitCounter'

# Depends on the source file (c)
task4.o: task4.c
	$(CC) $(CFLAGS) -m32 -fno-pie -o task4.o task4.c 

# Clean the build directory and executable
clean:
	rm -f *.o digitCounter
