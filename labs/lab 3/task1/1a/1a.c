#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int offsetsize = 18;

typedef struct virus {
    unsigned short SigSize;
    char virusName[16];
    char sig[];
} virus;

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

int main(int argc, char **argv) {
    FILE* file = fopen(argv[1], "r");
    if(file == NULL)
    {
        puts("no such input file");
        return -1;
    }
    fseek(file, 0, SEEK_END);
    size_t FileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    int offset = 0;
    while(offset < FileSize)
    {
        virus* v = readVirus(file);
        printVirus(v, stdout);
        offset += (offsetsize + v->SigSize);
        free(v);
    }
    fclose(file);
    return 0;
}