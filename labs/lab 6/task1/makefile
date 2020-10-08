# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = gcc
CFLAGS  = -g -Wall

# All Targets
all: myshell mypipe

# Tool invocations
# Executable "myshell" depends on the files:
myshell:  task1.o LineParser.o
	@echo 'Building target: myshell'
	$(CC) -m32 $(CFLAGS) -o myshell task1.o LineParser.o
	@echo 'Finished building target: myshell'

# Depends on the source file (c)
task1.o: task1.c
	$(CC) -m32 $(CFLAGS) -c -o task1.o task1.c

LineParser.o: LineParser.c
	$(CC) -m32 $(CFLAGS) -c -o LineParser.o LineParser.c

mypipe: mypipe.o
	$(CC) -m32 $(CFLAGS) -o mypipe mypipe.o

mypipe.o: mypipe.c
	$(CC) -m32 $(CFLAGS) -c -o mypipe.o mypipe.c


# Clean the build directory and executable
clean:
	rm -f *.o myshell mypipe
