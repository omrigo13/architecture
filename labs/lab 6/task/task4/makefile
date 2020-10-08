# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = gcc
CFLAGS  = -g -Wall

# All Targets
all: myshell

# Tool invocations
# Executable "myshell" depends on the files:
myshell:  task4.o LineParser.o
	@echo 'Building target: myshell'
	$(CC) -m32 $(CFLAGS) -o myshell task4.o LineParser.o
	@echo 'Finished building target: myshell'

# Depends on the source file (c)
task4.o: task4.c
	$(CC) -m32 $(CFLAGS) -c -o task4.o task4.c

LineParser.o: LineParser.c
	$(CC) -m32 $(CFLAGS) -c -o LineParser.o LineParser.c

# Clean the build directory and executable
clean:
	rm -f *.o myshell
