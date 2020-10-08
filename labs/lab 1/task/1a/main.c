#include <stdio.h>
#include <string.h>

const int asciichar = 'a' - 'A';

int main(int argc, char **argv) {
    FILE *f = stdin;
    int ch = fgetc(f);
    char c;
    if(argc != 1)
        return -1;
    while (ch != EOF)
    {
        if(ch <= 'z' && ch >= 'a')
            c = (char)(ch - asciichar);
        else
            c = (char)ch;
        printf("%c", c);
        ch = fgetc(f);
    }
    return 0;
}
