#include <stdio.h>
#include <string.h>

const int asciichar = 'a' - 'A';

int main(int argc, char **argv) {
    FILE *f;
    int ch, debug = 0;
    char c;
    if(argc > 2)
        return -1;    
    for(int i = 1; i < argc; i++)
        if(strcmp(argv[i],"-D") == 0)
            debug = 1;
    if(debug)
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
