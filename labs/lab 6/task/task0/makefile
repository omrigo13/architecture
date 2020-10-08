# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = gcc
CFLAGS  = -g -Wall

# All Targets
all: mypipe

# Tool invocations
# Executable "mypipe" depends on the files:
mypipe: mypipe.o LineParser.o
	@echo 'Building target: mypipe'
	$(CC) -m32 $(CFLAGS) -o mypipe mypipe.o LineParser.o
	@echo 'Finished building target: mypipe'

# Depends on the source file (c)

LineParser.o: LineParser.c
	$(CC) -m32 $(CFLAGS) -c -o LineParser.o LineParser.c

mypipe.o: mypipe.c
	$(CC) -m32 $(CFLAGS) -c -o mypipe.o mypipe.c


# Clean the build directory and executable
clean:
	rm -f *.o mypipe
