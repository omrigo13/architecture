#include <stdio.h>
#include <string.h>

const int asciichar = 'a' - 'A';
const int asciinum = '1' - 1;

int main(int argc, char **argv) {
    FILE *f;
    FILE *out = stdout;
    FILE *in = stdin;
    int ch, debug = 0, encoder = 0, fileout = 0, filein = 0;
    char c;
    char* encode;
    if(argc > 5)
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
        if(strncmp(argv[i], "-o", 2) == 0)
        {
            fileout = 1;
            out = fopen(argv[i] + 2, "w");
            if (out == NULL)
            {
                puts("can't write to file");
                return -1;
            }
        }
        if (strncmp(argv[i], "-i", 2) == 0)
        {
            filein = 1;
            in = fopen(argv[i] + 2, "r");
            if (in == NULL)
            {
                puts("no such input file");
                return -1;
            }
        }
    }
    if(encoder && debug)
    {
        for(int i = 1; i < argc; i++)
        {
            fprintf(stderr, "%s\n" ,argv[i]);
        }
        f = stdin;
        int ch, pos = 0, index = 2;
        char c;
        pos = encode[index] - asciinum;
        if(!filein)
            ch = fgetc(f);
        else
            ch = fgetc(in);
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
                if(!fileout)
                    printf("%c", c);
                else
                    fputc(c, out);
                if(!filein)
                    ch = fgetc(f);
                else
                    ch = fgetc(in);
            }
            fprintf(stderr,"\n");
            if(!fileout)
                printf("\n");
            else
                putc('\n', out);
            index = 2;
            pos = encode[index] - asciinum;
	        f = stdin;
	        if(!filein)
                ch = fgetc(f);
            else
                ch = fgetc(in);
        }
    }
    else if(encoder && !debug)
    {
        f = stdin;
        int ch, pos = 0, index = 2;
        char c;
        pos = encode[index] - asciinum;
        if(!filein)
            ch = fgetc(f);
        else
            ch = fgetc(in);
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
                if(!fileout)
                    printf("%c", c);
                else
                    fputc(c, out);
                if(!filein)
                    ch = fgetc(f);
                else
                    ch = fgetc(in);
            }
            if(!fileout)
                printf("\n");
            else
                putc('\n', out);
            index = 2;
            pos = encode[index] - asciinum;
            f = stdin;
	        if(!filein)
                ch = fgetc(f);
            else
                ch = fgetc(in);
        }
    }
    else if(debug && !encoder)
    {
        for(int i = 1; i < argc; i++)
        {
            fprintf(stderr, "%s\n" ,argv[i]);
        }
	    f = stdin;
	    if(!filein)
            ch = fgetc(f);
        else
        {
            ch = fgetc(in);
        }
	    while(ch != EOF)
	    {
	        while(ch != (char)'\n')
            {
                if(ch <= 'z' && ch >= 'a')
                    c = (char)(ch - asciichar);
                else
                    c = (char)ch;
                fprintf(stderr, "%d\t%d\n", ch, (int)c);
                if(!fileout)
                    printf("%c", c);
                else
                    fputc(c, out);
                if(!filein)
                    ch = fgetc(f);
                else
                    ch = fgetc(in);
            }
            fprintf(stderr,"\n");
            if(!fileout)
                printf("\n");
            else
                putc('\n', out);
	        f = stdin;
	        if(!filein)
                ch = fgetc(f);
            else
                ch = fgetc(in);
        }
    }
    else
    {
        f = stdin;
	    if(!filein)
            ch = fgetc(f);
        else
            ch = fgetc(in);
        while (ch != EOF)
        {
            if(ch <= 'z' && ch >= 'a')
                c = (char)(ch - asciichar);
            else
                c = (char)ch;
            if(!fileout)
                    printf("%c", c);
            else
                fputc(c, out);
            if(!filein)
                ch = fgetc(f);
            else
                ch = fgetc(in);
        }
    }
    fclose (out);
    fclose (in);
    return 0;
}