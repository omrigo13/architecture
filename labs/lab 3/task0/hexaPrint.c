#include <stdio.h>
#include <stdlib.h>

void printHex(char* buffer, int length) 
{
    for(int i = 0; i < length; i++)
        printf("%02hhX ", buffer[i]);
    printf("\n");
}

int main(int argc, char **argv) {
    FILE* file = fopen(argv[1], "r");
    char buffer[100];
    int n = 0;
    while(fread(&buffer[n], 1, 1, file) != 0){
        n++;
    }
    printHex(buffer, n);
    return 0;
}