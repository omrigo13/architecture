# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()

# All Targets
all: virus

# Tool invocations
# Executable "virus" depends on the files:
virus: task2.o
	@echo 'Building target: virus'
	gcc -nostdlib -m32 -Wall -g task2.o -o virus
	@echo 'Finished building target: virus'
	
# Depends on the source file (c)
task2.o: task2.s
	nasm -f elf task2.s -o task2.o

# Clean the build directory and executable
clean:
	rm -f *.o virus