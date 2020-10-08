# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = gcc
CFLAGS  = -g -Wall -c

# All Targets
all: my_echo

# Tool invocations
# Executable "lab1" depends on the files:
my_echo: main.o
	@echo 'Building target: my_echo'
	$(CC) -m32 main.o -o my_echo
	@echo 'Finished building target: my_echo'

# Depends on the source file (c)
main.o: main.c
	$(CC) $(CFLAGS) -m32 -o main.o main.c 

# Clean the build directory and executable
clean:
	rm -f *.o my_echo
