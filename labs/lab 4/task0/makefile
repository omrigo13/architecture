# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = gcc
CFLAGS  = -g -Wall -c

# All Targets
all: task0

# Tool invocations
# Executable "task0" depends on the files:
task0: main.o start.o util.o
	@echo 'Building target: task0'
	ld -m elf_i386 start.o main.o util.o -o task0
	@echo 'Finished building target: task0'

# Depends on the asm file (s)
start.o: start.s
	nasm -f elf start.s -o start.o

# Depends on the source file (c)
util.o: util.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o
	
main.o: main.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector main.c -o main.o

# Clean the build directory and executable
clean:
	rm -f *.o task0
