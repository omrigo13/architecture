# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = gcc
CFLAGS  = -g -Wall -c

# All Targets
all: lab1

# Tool invocations
# Executable "lab1" depends on the files:
lab1: add.o main.o numbers.o
	@echo 'Building target: lab1'
	$(CC) -m32 add.o main.o numbers.o -o lab1
	@echo 'Finished building target: lab1'

# Depends on the source file (c)
add.o: add.s
	nasm -g -f elf -w+all -o add.o add.s

main.o: add.s main.c
	$(CC) $(CFLAGS) -m32 -o main.o main.c 

numbers.o: main.c numbers.c
	$(CC) $(CFLAGS) -m32 -o numbers.o numbers.c 

# Clean the build directory and executable
clean:
	rm -f *.o lab1
