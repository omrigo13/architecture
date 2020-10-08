# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = gcc
CFLAGS  = -g -Wall

# All Targets
all: mypipeline

# Tool invocations
# Executable "mypipeline" depends on the files:
mypipeline:  task3.o LineParser.o
	@echo 'Building target: mypipeline'
	$(CC) -m32 $(CFLAGS) -o mypipeline task3.o LineParser.o
	@echo 'Finished building target: mypipeline'

# Depends on the source file (c)
task3.o: task3.c
	$(CC) -m32 $(CFLAGS) -c -o task3.o task3.c

LineParser.o: LineParser.c
	$(CC) -m32 $(CFLAGS) -c -o LineParser.o LineParser.c

# Clean the build directory and executable
clean:
	rm -f *.o mypipeline
