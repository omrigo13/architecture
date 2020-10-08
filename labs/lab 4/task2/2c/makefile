# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = gcc
CFLAGS  = -g -Wall -c

# All Targets
all: flame2

# Tool invocations
# Executable "flame2" depends on the files:
flame2: main.o start.o util.o
	@echo 'Building target: flame2'
	ld -m elf_i386 start.o main.o util.o -o flame2
	@echo 'Finished building target: flame2'

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
	rm -f *.o flame2