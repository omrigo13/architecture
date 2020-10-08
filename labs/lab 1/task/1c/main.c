#include <stdio.h>
#include <string.h>

const int asciichar = 'a' - 'A';
const int asciinum = '1' - 1;

int main(int argc, char **argv) {
    FILE *f;
    int ch, debug = 0, encoder = 0;
    char c;
    char* encode;
    if(argc > 3)
        return -1;
    for(int i = 1; i < argc; i++)
    {
        if(strcmp(argv[i],"-D") == 0)
            debug = 1;
        if (strncmp(argv[i], "+e", 2) == 0 || strncmp(argv[i], "-e", 2) == 0)
	    {
	        encoder = 1;
	        encode = argv[i];
	    }
    }
    if(encoder && debug)
    {
        for(int i = 1; i < argc; i++)
        {
            fprintf(stderr, "%s\n" ,argv[i]);
        }
        f = stdin;
        int ch = fgetc(f), pos = 0, index = 2;
        char c;
        pos = encode[index] - asciinum;
        while(ch != EOF)
	    {
	        while(ch != (char)'\n')
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
                fprintf(stderr, "%d\t%d\n", ch, (int)c);
                printf("%c", c);
                ch = fgetc(f);
            }
            fprintf(stderr,"\n");
            printf("\n");
            index = 2;
            pos = encode[index] - asciinum;
	        f = stdin;
	        ch = fgetc(f);
        }
 
    }
    else if(encoder && !debug)
    {
        f = stdin;
        int ch = fgetc(f), pos = 0, index = 2;
        char c;
        pos = encode[index] - asciinum;
        while(ch != EOF)
        {
            while(ch != (char)'\n')
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
                ch = fgetc(f);
            }
            printf("\n");
            index = 2;
            pos = encode[index] - asciinum;
            f = stdin;
	        ch = fgetc(f);
        }
    }
    else if(debug && !encoder)
    {
        for(int i = 1; i < argc; i++)
        {
            fprintf(stderr, "%s\n" ,argv[i]);
        }
	    f = stdin;
	    ch = fgetc(f);
	    while(ch != EOF)
	    {
	        while(ch != (char)'\n')
            {
                if(ch <= 'z' && ch >= 'a')
                    c = (char)(ch - asciichar);
                else
                    c = (char)ch;
                fprintf(stderr, "%d\t%d\n", ch, (int)c);
                printf("%c", c);
                ch = fgetc(f);
            }
            fprintf(stderr,"\n");
            printf("\n");
	        f = stdin;
	        ch = fgetc(f);
        }
    }
    else
    {
        f = stdin;
	    ch = fgetc(f);
        while (ch != EOF)
        {
            if(ch <= 'z' && ch >= 'a')
                c = (char)(ch - asciichar);
            else
                c = (char)ch;
            printf("%c", c);
            ch = fgetc(f);
        }
    }
    return 0;
}
