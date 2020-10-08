#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct virus {
    unsigned short SigSize;
    char virusName[16];
    char* sig;
} virus;

int detect_virus(char *buffer, unsigned int size, virus* vir)
{    
    for(int i = 0; i < size ; i++)
        if(i + vir->SigSize <= size)
            if(memcmp(&buffer[i], vir->sig, vir->SigSize) == 0)
            {
                printf("Starting byte: %d\nVirus name: %s\nVirus size: %d\n\n", i, vir->virusName, vir->SigSize);
                return 1;
            }
    return 0;
}

int main(int argc, char **argv) {
    FILE* file;
    int size;
    char buffer[10000];
    char entry0 = 0x80;
    char entry1 = 0x04;
    char entry2 = 0x08;
    virus* new_virus = (virus *)malloc(sizeof(virus));
    new_virus->SigSize = 15;
    char* name = "lab9";
    char* word = "This is a virus";
    strcpy(new_virus->virusName, name);
    new_virus->sig = word;
    file = fopen(argv[1], "r+");
    fseek(file, 0 , SEEK_END);
    size = ftell(file);
    fseek(file, 0 , SEEK_SET);
    fread(buffer, 1, size, file);
    if(detect_virus(buffer, size, new_virus))
    {
        fseek(file, 24, SEEK_SET);
        fwrite(&entry0, 1, 1, file);
        fwrite(&entry0, 1, 1, file);
        fwrite(&entry1, 1, 1, file);
        fwrite(&entry2, 1, 1, file);
    }
    fclose(file);
}