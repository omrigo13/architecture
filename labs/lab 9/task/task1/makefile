# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()

# All Targets
all: virus

# Tool invocations
# Executable "virus" depends on the files:
virus: task1.o
	@echo 'Building target: virus'
	gcc -nostdlib -m32 -Wall -g task1.o -o virus
	@echo 'Finished building target: virus'
	
# Depends on the source file (c)
task1.o: task1.s
	nasm -f elf task1.s -o task1.o

# Clean the build directory and executable
clean:
	rm -f *.o virus