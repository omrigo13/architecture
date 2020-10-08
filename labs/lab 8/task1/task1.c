#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define EI_DATA         5               /* Data encoding byte index */
#define ELFDATANONE     0               /* Invalid data encoding */
#define ELFDATA2LSB     1               /* 2's complement, little endian */
#define ELFDATA2MSB     2               /* 2's complement, big endian */
#define ELFDATANUM      3

typedef struct {
  char debug_mode;
  char file_name[128];
  int unit_size;
  unsigned char mem_buf[10000];
  size_t mem_count;
  int fd;               // file descriptor for the file
  void *map_start;      // will point to the start of the memory mapped file
  struct stat fd_stat;  // this is needed to  the size of the file
  Elf32_Ehdr *header;   // this will point to the header structure
  int num_of_section_headers;
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
void quit(state* s)
{
  /* is a function that prints "quitting" (in debug mode), and calls exit(0) to quit the program */
  if(s->debug_mode)
    fprintf(stderr, "quitting\n\n");
  free(s);
  exit(0);
}

char* encodingConvert(int n)
{
  switch (n)
  {
    case ELFDATANONE:
      return "Invalid data encoding";
      break;
    case ELFDATA2LSB:
      return "2's complement, little endian";
      break;
    case 2:
      return "2's complement, big endian";
      break;
    default:
      return "Invalid data encoding";
      break;
  }
}

void examineElf(state* s)
{
  /*
    s->header->e_ident - Magic number and other info
    s->header->e_entry - Entry point virtual address
    s->header->e_shoff - Section header table file offset
    s->header->e_shnum - Section header table entry count
    s->header->e_shentsize - Section header table entry size
    s->header->e_phoff - Program header table file offset
    s->header->e_phnum - Program header table entry count
    s->header->e_phentsize - Program header table entry size
  */
  printf("please enter a file name: ");
  fgets(s->file_name, 100, stdin);
  strtok(s->file_name, "\n");

  if(s->fd >= 0)
  {
    close(s->fd);
    s->fd = -1;
  }

  if((s->fd = open(s->file_name, O_RDWR)) < 0) 
  {
    perror("error while trying to open file");
    exit(-1);
  }

  if(fstat(s->fd, &(s->fd_stat)) != 0) 
  {
    perror("stat failed");
    close(s->fd);
    s->fd = -1;
    exit(-1);
  }
 
  if((s->map_start = mmap(0, s->fd_stat.st_size, PROT_READ , MAP_SHARED, s->fd, 0)) == MAP_FAILED) 
  {
    perror("mmap failed");
    close(s->fd);
    s->fd = -1;
    exit(-4);
  }
   
  s->header = (Elf32_Ehdr *) s->map_start;
  s->num_of_section_headers = s->header->e_shnum;
    
  if(strncmp((char*)(s->header->e_ident+1), "ELF", 3) != 0)
  {
    printf("illigal ELF file\n");
    munmap(s->map_start, s->fd_stat.st_size);
    close(s->fd);
    s->fd = -1;
  }
  else
    printf("ELF Header:\n\
    Magic:\t\t\t %X %X %X\n\
    Data:\t\t\t %s\n\
    Entry point address:\t %#x\n\
    Start of section headers:\t %d (bytes into file)\n\
    Number of section headers:\t %d\n\
    Size of section headers:\t %d (bytes)\n\
    Start of program headers:\t %d (bytes into file)\n\
    Number of program headers:\t %d\n\
    Size of program headers:\t %d (bytes)\n",\
    s->header->e_ident[1], s->header->e_ident[2], s->header->e_ident[3], encodingConvert(s->header->e_ident[5]), s->header->e_entry, s->header->e_shoff, s->header->e_shnum, s->header->e_shentsize, s->header->e_phoff, s->header->e_phnum, s->header->e_phentsize);

  if(s->debug_mode)
    fprintf(stderr, "\nDebug: file name set to: %s file descriptor: %d\n\n", s->file_name, s->fd);
}

void printSections(state* s)
{
  /*
    s->header->e_shoff - Section header table file offset
    s->header->e_shstrndx - Section header string table index
    headerTableStart[i].sh_name - Section name (string tbl index)
    headerTableStart[i].sh_addr - Section virtual addr at execution
    headerTableStart[i].sh_offset - Section file offset
    headerTableStart[i].sh_size - Section size in bytes
    headerTableStart[i].sh_type - Section type
  */
  Elf32_Shdr* headerTableStart = s->map_start + s->header->e_shoff;
  char* sectionOffset = s->map_start + (headerTableStart + s->header->e_shstrndx)->sh_offset;
  printf("Section Headers:\n");
  printf("\t[Index]\tsection_name\t\tsection_address\t  section_offset   section_size\t  section_type\n");
  int index = -1, address = -1, offset = -1, size = -1, type = -1;
  char* name;
  for (int i = 0; i < s->header->e_shnum; i++)
  {
    index = i;
    name = sectionOffset + headerTableStart[i].sh_name;
    address = headerTableStart[i].sh_addr;
    offset = headerTableStart[i].sh_offset;
    size = headerTableStart[i].sh_size;
    type = headerTableStart[i].sh_type;
    printf("\t%d\t%-16s\t%08x\t  %d\t\t   %d\t\t  %x\n", index, name, address, offset, size, type);
  }
}

struct fun_desc {
  char *name;
  void (*fun)(state*);
};

int main(int argc, char **argv){
  struct fun_desc menu[] = { { "Toggle Debug Mode", debugFlag }, { "Examine ELF File", examineElf }, 
  {"Print Section Names", printSections}, { "Quit", quit }, { NULL, NULL } };
  int choice = -1, index = 0;
  char input[100];
  state* s = calloc(1, sizeof(state));
  s->unit_size = 1;
  s->fd = -1;
  while((menu+index)->name != NULL)
    index++;
  index -= 1;
  while(1)
  {
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