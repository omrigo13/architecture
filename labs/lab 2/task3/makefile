# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = gcc
CFLAGS  = -g -Wall -c

# All Targets
all: menu_map

# Tool invocations
# Executable "menu_map" depends on the files:
menu_map: menu_map.o
	@echo 'Building target: menu_map'
	$(CC) -m32 menu_map.o -o menu_map
	@echo 'Finished building target: menu_map'

# Depends on the source file (c)
menu_map.o: menu_map.c
	$(CC) $(CFLAGS) -m32 -o menu_map.o menu_map.c 

# Clean the build directory and executable
clean:
	rm -f *.o menu_map
