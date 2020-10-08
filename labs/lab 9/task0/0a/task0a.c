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

int main(int argc, char **argv){
  state* s = malloc(sizeof(state));
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
  Elf32_Phdr* programHeaderStart = s->map_start + s->header->e_phoff;
  printf("Program Headers:\n");
  printf("\tType\t\tOffset\t\tVirtAddr\tPhysAddr\tFileSiz\tMemSiz\tFlg\tAlign\n");
  int type = -1, offset = -1, virtualAddr = -1, physicalAddr = -1, fileSize = -1, memSize, flg = -1, allign = -1;
  for(int i = 0; i < s->header->e_phnum; i++)
  {
    type = programHeaderStart->p_type;
    offset = programHeaderStart->p_offset;
    virtualAddr = programHeaderStart->p_vaddr;
    physicalAddr = programHeaderStart->p_paddr;
    fileSize = programHeaderStart->p_filesz;
    memSize = programHeaderStart->p_memsz;
    flg = programHeaderStart->p_flags;
    allign = programHeaderStart->p_align;
    printf("\t%d\t\t%#08x\t%#08x\t%#08x\t%#08x\t%#08x\t%d\t%#x\n", type, offset, virtualAddr, physicalAddr, fileSize, memSize, flg, allign);
    programHeaderStart++;
  }
  if(s->debug_mode)
    fprintf(stderr, "\nDebug: file name set to: %s file descriptor: %d\n\n", s->file_name, s->fd);
  free(s);
  return 0;
}