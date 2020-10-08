#include <stdlib.h>
#include <stdio.h>
#include <string.h>
 
char censor(char c) {
  if(c == '!')
    return '.';
  else
    return c;
}
 
char* map(char *array, int array_length, char (*f) (char)){
  char* mapped_array = (char*)(malloc(array_length*sizeof(char)));
  /* TODO: Complete during task 2.a */
  for(int i = 0; i < array_length; i++)
      mapped_array[i] = f(*(array + i));
  return mapped_array;
}
 
char encrypt(char c){
/* Gets a char c and returns its encrypted form by adding 3 to its value. 
          If c is not between 0x20 and 0x7E it is returned unchanged */
  if(c >= 0x20 && c <= 0x7E)
    c = c + 3;
  return c;
} 
char decrypt(char c){
/* Gets a char c and returns its decrypted form by reducing 3 to its value. 
            If c is not between 0x20 and 0x7E it is returned unchanged */
  if(c >= 0x20 && c <= 0x7E)
    c = c - 3;
  return c;
} 
char dprt(char c){
/* dprt prints the value of c in a decimal representation followed by a 
           new line, and returns c unchanged. */
  printf("%d\n",c);
  return c;
} 
char cprt(char c){
/* If c is a number between 0x20 and 0x7E, cprt prints the character of ASCII value c followed 
                    by a new line. Otherwise, cprt prints the dot ('.') character. After printing, cprt returns 
                    the value of c unchanged. */
  if(c >= 0x20 && c <= 0x7E)
    printf("%c\n", c);
  else
    printf(".\n");
  return c;
} 
char my_get(char c){
/* Ignores c, reads and returns a character from stdin using fgetc. */
  return fgetc(stdin);
}

char quit(char c){
/* Gets a char c,  and if the char is 'q' , ends the program with exit code 0. Otherwise returns c. */
  if(c == 'q')
    exit(0);
  else
    return c;
}

struct fun_desc {
  char *name;
  char (*fun)(char);
};

struct fun_desc menu[] = { { "Censor", censor }, { "Encrypt", encrypt }, { "Decrypt", decrypt }, { "Print dec", dprt }, { "Print string", cprt },
{ "Get string", my_get }, { "Quit", quit }, { NULL, NULL } };

int MenuSize()
{
  int size = 0;
  while(menu[size].name != NULL)
    size++;
  return size;
}

void MenuDisplay()
{
  int i = 0;
  printf("Please choose a function:\n");
  while(menu[i].name != NULL)
  {
    printf("%d) %s\n", i, menu[i].name);
    i++;
  }
  printf("Option: ");
}

int main(int argc, char **argv){
  /* TODO: Test your code */
  char* carray = (char*)calloc(5, sizeof(char));
  int option;
  MenuDisplay();
  option = fgetc(stdin) - '0';
  while(option >= 0 && option < MenuSize())
  {
    fgetc(stdin);
    printf("Within bounds\n");
    carray = map(carray, 5, menu[option].fun);
    printf("DONE.\n\n");
    MenuDisplay();
    option = fgetc(stdin) - '0';
  }
  printf("Not within bounds");
  free(carray);
  return 0;
}