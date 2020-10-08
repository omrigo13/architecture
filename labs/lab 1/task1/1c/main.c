#include <stdio.h>
#include <string.h>
#include <limits.h>

const int asciichar = 32;
const int asciinum = 48;

void task1a(char* word)
{
    int ch = word[0], index = 0;
    char c;
    while (ch != EOF)
    {
        while(index < strlen(word))
        {
            if(ch <= 122 && ch >= 97)
                c = (char)(ch - asciichar);
            else
                c = (char)ch;
            printf("%c", c);
            ch = word[++index];
        }
        ch = word[++index];
    }
}

void task1b(char* word)
{
    int ch = word[0], index = 0;
    char c;
    while(ch != 0)
    {
        if (ch <= 122 && ch >= 97)
            c = (char) (ch - asciichar);
        else
            c = (char) ch;
        printf("%d\t%d\n", ch, (int) c);
        ch = word[++index];
    }
    printf("\n");
    task1a(word);
    printf("\n");
}

void task1c(char* word, char* encode)
{
    int ch = word[0], pos = 0, i = 0, index = 2;
    char c;
    pos = encode[index] - asciinum;
    while(ch != 0)
    {
        if(strncmp(encode, "+e", 2) == 0)
            c = (char)(ch + pos);
        if(strncmp(encode, "-e", 2) == 0)
            c = (char)(ch - pos);
        if(pos == encode[strlen(encode) - 1] - asciinum)
        {
            index = 2;
            pos = encode[index] - asciinum;
        }
        else
            pos = encode[++index] - asciinum;
        printf("%c", c);
        ch = word[++i];
    }
    printf("\n");
}

int main(int argc, char **argv) {
    char word [MAX_INPUT];
    int i = 0;
    char c;
    if(argc == 1)
    {
        while(1)
        {
            while (1) {
                word[i] = fgetc(stdin);
                if (word[i] == (char) '\n')
                    break;
                ++i;
            }
            word[i] = 0;
            task1a(word);
            printf("\n");
            i = 1;
            c = fgetc(stdin);
            if(c == EOF)
                break;
            else
                word[0] = c;
        }
    }
    if(argc == 2 && strcmp(argv[1],"-D") == 0)
    {
        fprintf(stderr, "%s\n" ,argv[1]);
        while(1)
        {
            while (1) {
                word[i] = fgetc(stdin);
                if (word[i] == (char) '\n')
                    break;
                ++i;
            }
            word[i] = 0;
            task1b(word);
            i = 1;
            c = fgetc(stdin);
            if(c == EOF)
                break;
            else
                word[0] = c;
        }
    }
    if (argc == 2 && strncmp(argv[1], "+e", 2) == 0)
    {
        char* encode = argv[1];
        while(1)
        {
            while (1) {
                word[i] = fgetc(stdin);
                if (word[i] == (char) '\n')
                    break;
                ++i;
            }
            word[i] = 0;
            task1c(word, encode);
            i = 1;
            c = fgetc(stdin);
            if(c == EOF)
                break;
            else
                word[0] = c;
        }
    }
    else if (argc == 2 && strncmp(argv[1], "-e", 2) == 0)
    {
        char* encode = argv[1];
        while(1)
        {
            while (1) {
                word[i] = fgetc(stdin);
                if (word[i] == (char) '\n')
                    break;
                ++i;
            }
            word[i] = 0;
            task1c(word, encode);
            i = 1;
            c = fgetc(stdin);
            if(c == EOF)
                break;
            else
                word[0] = c;
        }
    }
    return 0;
}
