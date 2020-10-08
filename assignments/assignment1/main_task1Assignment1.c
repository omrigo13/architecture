#include <stdio.h>
#include <stdlib.h>
#define	MAX_LEN 34			/* maximal input string size */
					/* enough to get 32-bit string + '\n' + null terminator */
extern void assFunc(int x, int y);

char c_checkValidity(int x, int y)
{
  if(x >= y)
    return 1;
  return 0;
}

int main(int argc, char** argv)
{
  char buf[MAX_LEN];
  fgets(buf, MAX_LEN, stdin);		/* get user first input */
  int x = atoi(buf); 
  fgets(buf, MAX_LEN, stdin);		/* get user second input */
  int y = atoi(buf);
  assFunc(x,y);			/* call your assembly function */
  return 0;
}