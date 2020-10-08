#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int offsetsize = 18;
int zerochar = '0';

typedef struct virus {
    unsigned short SigSize;
    char virusName[16];
    char sig[];
} virus;

typedef struct link link;

struct link {
    link *nextVirus;
    virus *vir;
};

struct fun_desc {
  char *name;
  char (*fun)(char);
};

link* listOfViruses = NULL;

virus* readVirus(FILE* file)
{
/* gets a file pointer and return a virus* represents the next virus in the file*/
    unsigned char size[2];
    unsigned short newSigSize;
    virus* new_virus = NULL;
    if (fread(size, 1, 2, file) != 0)
    {
        newSigSize = *(unsigned short *)size;
        new_virus = (virus *)malloc(newSigSize + sizeof(char[16]) + sizeof(virus));
        new_virus->SigSize = newSigSize;
        fread(new_virus->virusName, 1, 16, file);
        fread(new_virus->sig, 1, new_virus->SigSize, file);
    }
    return new_virus;
}

void printHex(char* buffer, int length, FILE* output) 
{
    for(int i = 0; i < length; i++)
        fprintf(output, "%02hhX ", buffer[i]);
    fprintf(output, "\n");
}

void printVirus(virus* virus, FILE* output)
{
/* gets a virus and a pointer to an output file. prints the virus to the given output
   virus name in ascii
   virus signature length in decimal
   virus signature in hexadecimal
*/
    fprintf(output, "Virus name: %s\nVirus size: %d\nsignature:\n", virus->virusName, virus->SigSize);
    printHex(virus->sig, virus->SigSize, output);
    fprintf(output, "\n");
}

void list_print(link* virus_list, FILE* file)
{
/* Print the data of every link in list to the given stream. Each item followed by a newline character. */
    while(virus_list != NULL)
    {
        printVirus(virus_list->vir, file);
        virus_list = virus_list->nextVirus;
    }   
}

link* list_append(link* virus_list, virus* data)
{
/* Add a new link with the given data to the list 
        (either at the end or the beginning, depending on what your TA tells you),
        and return a pointer to the list (i.e., the first link in the list).
        If the list is null - create a new entry and return a pointer to the entry. */
    if(virus_list != NULL)
    {
        link* head = (link*)malloc(sizeof(link));
        head->vir = data;
        head->nextVirus = virus_list;
        return head;
    }
    else
    {
        virus_list = (link*)calloc(sizeof(link),1);
        virus_list->vir = data;
        virus_list->nextVirus = NULL;
        return virus_list;
    }
} 

void list_free(link *virus_list)
{
/* Free the memory allocated by the list. */
    while(virus_list != NULL)
    {
        link* FreeViruses = virus_list;
        virus_list = virus_list->nextVirus;
        free(FreeViruses->vir);
        free(FreeViruses);
    }
}    

void detect_virus(char *buffer, unsigned int size, link *virus_list)
{
/* compares the buffer with the viruses signatures stored in the virus list byte by byte */
    while (virus_list != NULL)
    {
        for (int i = 0; i < size; i++)
            if (memcmp(&buffer[i], virus_list->vir->sig, virus_list->vir->SigSize) == 0)
                printf("location: %02X\nvirus name: %s\nVirus size: %d\n\n", i, virus_list->vir->virusName, virus_list->vir->SigSize);
        virus_list = virus_list->nextVirus; 
    }
}

void detect()
{
    char buffer[10000];
    char fileName[100];
    int size = 0;
    FILE* file = NULL;

    printf("Please enter a suspected file name: ");
    fgets(fileName, 100, stdin);
    sscanf(fileName, "%s", fileName);
    file = fopen(fileName, "r");
    if(size > sizeof(buffer))
    {
        printf("too big file");
        exit(-1);
    }
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);
    fread(buffer, 1, size, file);
    detect_virus(buffer, size, listOfViruses);
    fclose(file);
}

//starting at 507 and size 12 for remove I am virus1!
void kill_virus(char *fileName, int signitureOffset, int signitureSize)
{
/* fixes a virus by getting starting point and size of virus */
    char nop = 0x0;
    FILE* file = NULL;
    file = fopen(fileName,"r+");
    if(file == NULL) 
    {
        puts("can't read from a file");
        exit(-1);
    }
    fseek(file, signitureOffset, SEEK_SET);
    for(int i = 0; i < signitureSize; i++)
    {
        fwrite(&nop, 1, 1, file);
        fseek(file, signitureOffset + i + 1, SEEK_SET);
    }
    fclose(file);
}

void fix()
{
    char fileName[100];
    char buffer[100];
    int offset = 0;
    int size = 0;
    printf("Please enter a suspected file's name: ");
    fgets(fileName, 100, stdin);
    sscanf(fileName, "%s", fileName);
    printf("Please enter the virus's starting byte: ");
    fgets(buffer, 100, stdin);
    sscanf(buffer, "%d", &offset);
    printf("Please enter the virus's signature size: ");
    fgets(buffer, 100, stdin);
    sscanf(buffer, "%d", &size);
    kill_virus(fileName, offset, size);
}

void quit()
{
    list_free(listOfViruses);
    exit(0);
}

void loadSigs()
{
    char fileName[100];
    FILE* file = NULL;
    virus* vir = NULL;

    printf("Please enter a signatures file name: ");
    fgets(fileName, 100, stdin);
    sscanf(fileName, "%s", fileName);
    file = fopen(fileName, "r");
    if (file != NULL){
        while((vir = readVirus(file)) != NULL){
            listOfViruses = list_append(listOfViruses, vir);
        }
    }
    fclose(file);
}

void printSigs()
{
    list_print(listOfViruses, stdout);
}


struct fun_desc menu[] = { { "Load signatures", loadSigs }, { "Print signatures", printSigs }, {"Detect viruses", detect}, {"Fix file", fix}, { "Quit", quit }, { NULL, NULL } };

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
    printf("%d) %s\n", (i+1), menu[i].name);
    i++;
  }
  printf("Option: ");
}

int main(int argc, char **argv) {
    int choice;
    MenuDisplay();
    choice = fgetc(stdin) - zerochar - 1;
    while(choice >= 0 && choice < MenuSize())
    {
        fgetc(stdin);
        printf("Within bounds\n");
        (menu + choice)->fun(listOfViruses);
        printf("DONE.\n\n");
        MenuDisplay();
        choice = fgetc(stdin) - zerochar - 1;
    }
    printf("Not within bounds");
    return 0;
}