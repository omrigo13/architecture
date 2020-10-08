#include <stdio.h>

int digit_cnt(char* input) 
{
  int i, count = 0;
  for(i=0; input[i]; i++) 
    if (input[i] >= '0' && input[i] <= '9')
      count++;  
  return count;
}
 
int main(int argc, char **argv){
  if(argc < 2)
    printf("usage: ntsc <string>\n");
  else 
    printf("The number of digits in the string is: %d\n", digit_cnt(argv[1]));
  return 0;
}