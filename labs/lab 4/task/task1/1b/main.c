#include "util.h"

#define SYS_READ 3
#define SYS_WRITE 4
#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define O_RDONLY 0
#define O_WRONLY 1
#define O_CREAT 64
#define O_TRUNC 512

extern int system_call();

int diff = 'a' - 'A';
char* inName = "stdin";
char* outName = "stdout";

void debug(int id, int code)
{
  system_call(SYS_WRITE, STDERR, "ID: ", 4);
  system_call(SYS_WRITE, STDERR, itoa(id), strlen(itoa(id)));
  system_call(SYS_WRITE, STDERR, ", return code: ", strlen(", return code: "));
  system_call(SYS_WRITE, STDERR, itoa(code), strlen(itoa(code)));
  system_call(SYS_WRITE, STDERR, ", in file path: ", 16);
  system_call(SYS_WRITE, STDERR, inName, strlen(inName));
  system_call(SYS_WRITE, STDERR, ", out file path: ", 17);
  system_call(SYS_WRITE, STDERR, outName, strlen(outName));
  system_call(SYS_WRITE, STDERR, "\n", 2);
}

int main (int argc , char* argv[], char* envp[])
{
  char debugMode = 0, c;
  int in = STDIN, out = STDOUT, code = 0, i;
  char buffer[1];

  for(i = 1; i < argc; i++)
  {
    if(strcmp(argv[i], "-D") == 0)
      debugMode = 1;
    else if ((strncmp(argv[i], "-i", 2) == 0))
    {
      inName = argv[i] + 2;
      in = system_call(SYS_OPEN, argv[i] + 2, O_RDONLY, 0777);
      if(in < 0)
      {
        system_call(SYS_WRITE, STDERR, "cannot open input file\n", 24);
        return 1;
      }
      else if(debugMode)
        debug(SYS_OPEN, in);
    }
    else if((strncmp(argv[i], "-o", 2) == 0))
    {
      outName = argv[i] + 2;
      out = system_call(SYS_OPEN, argv[i] + 2, O_WRONLY | O_CREAT | O_TRUNC, 0777);
      if(out < 0)
      {
        system_call(SYS_WRITE, STDERR, "cannot write to output file\n", 29);
        return 1;
      }
      else if(debugMode)
        debug(SYS_OPEN, out);
    }
  }
  
  while((code = system_call(SYS_READ, in, &c, 1)) > 0 && c != -1)
  {
    if (debugMode)
      debug(SYS_READ, code);
    if ( c >= 'a' && c <= 'z')
      c -= diff;
    buffer[0] = c;
    code = system_call(SYS_WRITE, out, buffer, 1);
    if (debugMode)
      debug(SYS_WRITE, code);
  }

  if(out != STDOUT)
  {
    code = system_call(SYS_CLOSE, out);
    if(code < 0)
        system_call(SYS_WRITE, STDERR, "error while trying to close output file\n", 41);
    else if(debugMode)
        debug(SYS_CLOSE, code);
  }
  if(in != STDIN)
  {
    code = system_call(SYS_CLOSE, in);
    if(code < 0)
        system_call(SYS_WRITE, STDERR, "error while trying to close input file\n", 40);
    else if(debugMode)
        debug(SYS_CLOSE, code);
  }

  return 0;
}