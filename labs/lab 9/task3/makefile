# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()

# All Targets
all: virus

# Tool invocations
# Executable "virus" depends on the files:
virus: task3.o
	@echo 'Building target: virus'
	gcc -nostdlib -m32 -Wall -g task3.o -o virus
	@echo 'Finished building target: virus'
	
# Depends on the source file (c)
task3.o: task3.s
	nasm -f elf task3.s -o task3.o

# Clean the build directory and executable
clean:
	rm -f *.o virus