#include <stdio.h>
#include <string.h>
#include <limits.h>

const int asciichar = 32;
const int asciinum = 48;

void task1a(char* word, FILE *f, char fileopt)
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
            if(fileopt)
                fputc(c, f);
            else
                printf("%c", c);
            ch = word[++index];
        }
        ch = word[++index];
    }
    if(fileopt)
        fputc('\n', f);
}

void task1b(char* word, FILE *f, char fileopt, char encoder, char* encode)
{
    int ch = word[0], index = 0;
    char c;
    if(!encoder)
    {
	while(ch != 0)
	{
            if (ch <= 122 && ch >= 97)
                c = (char) (ch - asciichar);
            else
                c = (char) ch;
            if(fileopt)
                fprintf(f,"%d\t%d\n", ch, (int) c);
            else
                printf("%d\t%d\n", ch, (int) c);
            ch = word[++index];
        }
    }
    else
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
            if(fileopt)
            	fprintf(f,"%d\t%d\n", ch, (int) c);
            else
            	printf("%d\t%d\n", ch, (int) c);
            ch = word[++i];
        }
    }
    if(!fileopt)
        printf("\n");
    else
        fputc('\n', f);
    if(!encoder)
	task1a(word, f, fileopt);
    if(!fileopt && !encoder)
        printf("\n");
    if(fileopt && !encoder)
        fputc('\n', f);
}

void task1c(char* word, char* encode, FILE *f, char fileopt)
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
        if(fileopt)
            fputc(c, f);
        else
            printf("%c", c);
        ch = word[++i];
    }
    if(!fileopt)
        printf("\n");
    else
        fputc('\n', f);
}

int main(int argc, char **argv) {
    char word [MAX_INPUT];
    int i = 0;
    char c, fileopt = 0, debug = 0, encoder = 0;
    char* encode;
    FILE* out = stdout;
    for(int j = 1; j < argc; j++)
    {
        if(strcmp(argv[j],"-D") == 0)
            debug = 1;
        if (strncmp(argv[j], "+e", 2) == 0 || strncmp(argv[j], "-e", 2) == 0)
	{
	    encoder = 1;
	    encode = argv[j];
	}
        if(strncmp(argv[j], "-o", 2) == 0)
        {
            fileopt = 1;
            out = fopen(argv[j] + 2, "w");
            if (out == NULL)
            {
                puts ("can't write to file");
                return -1;
            }
        }
    }
    if(!debug && !encoder)
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
            task1a(word, out, fileopt);
            if(!fileopt)
                printf("\n");
            i = 1;
            c = fgetc(stdin);
            if(c == EOF)
                break;
            else
                word[0] = c;
        }
    }
    else if(debug && !encoder)
    {
        for(int j = 1; j < argc; j++)
            fprintf(stderr, "%s\n" ,argv[j]);
        while(1)
        {
            while (1) {
                word[i] = fgetc(stdin);
                if (word[i] == (char) '\n')
                    break;
                ++i;
            }
            word[i] = 0;
            task1b(word, out, fileopt, encoder, encode);
            i = 1;
            c = fgetc(stdin);
            if(c == EOF)
                break;
            else
                word[0] = c;
        }
    }
    else if (encoder)
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
	    if(debug)
	    {
		task1b(word, out, fileopt, encoder, encode);
		task1c(word, encode, out, fileopt);
	    }
	    else
		task1c(word, encode, out, fileopt);
            i = 1;
            c = fgetc(stdin);
            if(c == EOF)
                break;
            else
                word[0] = c;
        }
    }
    fclose (out);
    return 0;
}

