# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = gcc
CFLAGS  = -g -Wall

# All Targets
all: myshell

# Tool invocations
# Executable "myshell" depends on the files:
myshell:  task2.o LineParser.o
	@echo 'Building target: myshell'
	$(CC) -m32 $(CFLAGS) -o myshell task2.o LineParser.o
	@echo 'Finished building target: myshell'

# Depends on the source file (c)
task2.o: task2.c
	$(CC) -m32 $(CFLAGS) -c -o task2.o task2.c

LineParser.o: LineParser.c
	$(CC) -m32 $(CFLAGS) -c -o LineParser.o LineParser.c

looper: looper.o
	$(CC) -m32 $(CFLAGS) -o looper looper.o

looper.o: looper.c
	$(CC) -m32 $(CFLAGS) -c -o looper.o looper.c


# Clean the build directory and executable
clean:
	rm -f *.o myshell
