# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = gcc
CFLAGS  = -g -Wall -c

# All Targets
all: count-words

# Tool invocations
# Executable "count-words" depends on the files:
count-words: count-words.o
	@echo 'Building target: count-words'
	$(CC) -m32 count-words.o -o count-words
	@echo 'Finished building target: count-words'

# Depends on the source file (c)
count-words.o: count-words.c
	$(CC) $(CFLAGS) -m32 -o count-words.o count-words.c 

# Clean the build directory and executable
clean:
	rm -f *.o count-words
