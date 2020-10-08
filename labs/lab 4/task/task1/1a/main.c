#include "util.h"

#define SYS_READ 3
#define SYS_WRITE 4
#define STDIN 0
#define STDOUT 1
#define STDERR 2

extern int system_call();

int diff = 'a' - 'A';

void debug(int id, int code)
{
  system_call(SYS_WRITE, STDERR, "ID: ", 4);
  system_call(SYS_WRITE, STDERR, itoa(id), strlen(itoa(id)));
  system_call(SYS_WRITE, STDERR, ", return code: ", strlen(", return code: "));
  system_call(SYS_WRITE, STDERR, itoa(code), strlen(itoa(code)));
  system_call(SYS_WRITE, STDERR, "\n", 2);
}

int main (int argc , char* argv[], char* envp[])
{
  char debugMode = 0, c;
  int in = STDIN, out = STDOUT, code = 0, i;
  char buffer[1];

  for(i = 1; i < argc; i++)
    if(strcmp(argv[i], "-D") == 0)
      debugMode = 1;

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

  return 0;
}