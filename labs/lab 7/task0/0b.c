#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char debug_mode;
  char file_name[128];
  int unit_size;
  unsigned char mem_buf[10000];
  size_t mem_count;
  /*
   .
   .
   Any additional fields you deem necessary
  */
} state;

void debugFlag(state* s)
{
  /* Toggle debug mode means turn the debug flag on or off */
  s->debug_mode = 1 - s->debug_mode;
  if(s->debug_mode)
    printf("Debug flag now on\n");
  else
    printf("Debug flag now off\n");
}

void setFile(state* s)
{
  /* queries the user for a file name, and store it in file_name */
  printf("please enter a file name: ");
  fgets(s->file_name, 100, stdin);
  strtok(s->file_name, "\n");
  if(s->debug_mode)
    fprintf(stderr, "Debug: file name set to: %s\n\n", s->file_name);
}

void setUnitSize(state* s)
{  
  /* option sets the size variable */  
  char size[3];
  printf("please enter a unit size (1, 2 or 4): ");
  fgets(size, 3, stdin);
  if (strncmp(size, "1", 1) == 0 || strncmp(size, "2", 1) == 0 || strncmp(size, "4", 1) == 0)
  {
    sscanf(size, "%d", &(s->unit_size));
    if(s->debug_mode)
      fprintf(stderr, "Debug: set size to: %d\n\n", s->unit_size);   
  }
  else
    printf("the size arguments is illigal: %s", size); 
}

void quit(state* s)
{
  /* is a function that prints "quitting" (in debug mode), and calls exit(0) to quit the program */
  if(s->debug_mode)
    fprintf(stderr, "quitting\n\n");
  free(s);
  exit(0);
}

struct fun_desc {
  char *name;
  void (*fun)(state*);
};

int main(int argc, char **argv){
  struct fun_desc menu[] = { { "Toggle Debug Mode", debugFlag }, { "Set File Name", setFile }, { "Set Unit Size", setUnitSize }, { "Quit", quit }, { NULL, NULL } };
  int choice = -1, index = 0;
  char input[100];
  state* s = calloc(1, sizeof(state));
  s->unit_size = 1;
  while((menu+index)->name != NULL)
    index++;
  index -= 1;
  while(1)
  {
    if(s->debug_mode)
      fprintf(stderr, "Debug: unit size: %d, file name: %s, mem count: %u\n\n", s->unit_size, s->file_name, s->mem_count);    
    printf("Choose action:\n");
    for(int i = 0; (menu + i)->name != NULL; i++) 
      printf("%d-%s\n", i, (menu + i)->name);
    printf("option: ");
    fgets(input, 100, stdin);
    if(strcmp(input, "\n") == 0)
      continue;
    choice = -1;
    sscanf(input, "%d", &choice);
    if(choice < 0 || choice > index)
    {
      printf("Not within bounds\n");
      quit(s);
    }
    printf("Within bounds\n");     
    (menu + choice)->fun(s);
    printf("DONE.\n\n");
  }
  return 0;
}