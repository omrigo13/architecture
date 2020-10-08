# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = gcc
CFLAGS  = -g -Wall -c

# All Targets
all: addresses

# Tool invocations
# Executable "addresses" depends on the files:
addresses: addresses.o
	@echo 'Building target: addresses'
	$(CC) -m32 addresses.o -o addresses
	@echo 'Finished building target: addresses'

# Depends on the source file (c)
addresses.o: addresses.c
	$(CC) $(CFLAGS) -m32 -o addresses.o addresses.c 

# Clean the build directory and executable
clean:
	rm -f *.o addresses
