#include <stdio.h>

int main (int argc, char** argv){
    int iarray[] = {1,2,3};
    char carray[] = {'a','b','c'};
    int* iarrayPtr = iarray;
    char* carrayPtr = carray;
    for(int i = 0;i < 3;i++)
        printf("%i", *(iarrayPtr + i));
    printf("\n");
    for(int i = 0;i < 3;i++)
        printf("%c", *(carrayPtr + i));
    printf("\n");
    void* p;
    printf("%p\n", p); //address of pointer to p
    return 0;
}