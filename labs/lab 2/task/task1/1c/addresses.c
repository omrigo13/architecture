#include <stdio.h>

int main (int argc, char** argv){
    int iarray[3];
    float farray[3];
    double darray[3];
    char carray[3];
    printf("%d\n", iarray);
    printf("%d\n", iarray + 1); //diffrence of 4 byte because it's int array
    printf("%d\n", farray);
    printf("%d\n", farray + 1); //diffrence of 4 byte because it's float array
    printf("%d\n", darray);
    printf("%d\n", darray + 1); //diffrence of 8 byte because it's double array
    printf("%d\n", carray);
    printf("%d\n", carray + 1); //diffrence of 1 byte because it's char array
    return 0;
}