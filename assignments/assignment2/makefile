# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = gcc

# All Targets
all: calc

# Tool invocations
# Executable "calc" depends on the files:
calc: calc.o
	@echo 'Building target: calc'
	$(CC) -m32 -g -Wall calc.o -o calc
	@echo 'Finished building target: calc'
	
# Depends on the source file (c)
calc.o: calc.s
	nasm -f elf calc.s -o calc.o

# Clean the build directory and executable
clean:
	rm -f *.o calc