#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    FILE *f = stdin;
    int ch = fgetc(f);
    char c;
    if(argc == 1)
    {
        printf("#> encoder\n");
        while (ch != EOF)
        {
            if(ch <= 122 && ch >= 97)
                c = (char)(ch - 32);
            else
                c = (char)ch;
            printf("%c", c);
            ch = fgetc(f);
        }
    }
    return 0;
}
