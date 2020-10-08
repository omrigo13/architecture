# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()

# All Targets
all: task0b

# Tool invocations
# Executable "task0b" depends on the files:
task0b: task0b.o
	@echo 'Building target: task0b'
	gcc -nostdlib -m32 -Wall -g task0b.o -o task0b
	@echo 'Finished building target: task0b'
	
# Depends on the source file (c)
task0b.o: task0b.s
	nasm -f elf task0b.s -o task0b.o

# Clean the build directory and executable
clean:
	rm -f *.o task0b