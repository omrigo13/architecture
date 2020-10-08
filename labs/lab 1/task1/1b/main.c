#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    FILE *f;
    int ch;
    char c;
    if(argc == 1)
    {
        printf("#> encoder\n");
	f = stdin;
	ch = fgetc(f);
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
    else if(argc == 2 && strcmp(argv[1],"-D") == 0)
    {
        printf("#> encoder -D\n");
        fprintf(stderr, "%s\n" ,argv[1]);
	f = stdin;
	ch = fgetc(f);
	while(ch != EOF)
	{
	    while(ch != (char)'\n')
            {
                if(ch <= 122 && ch >= 97)
                    c = (char)(ch - 32);
                else
                    c = (char)ch;
                printf("%d\t%d\n", ch, (int)c);
                ch = fgetc(f);
            }
	    f = stdin;
	    ch = fgetc(f);
	}
    }
    return 0;
}
