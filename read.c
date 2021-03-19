#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

char *readlineGZ(gzFile file)
{
    size_t baselen = 256;
    char *line = (char *)malloc(sizeof(char) * baselen);
    if(!line) exit(1);

    size_t index = 0;
    int ch;
    while((ch=gzgetc(file)) != -1 && ch != 10) // 10 => "\n"
    {
        line[index] = ch;
        index++;

        if(index == baselen)
        {
            baselen += 128;
            line = (char *)realloc(line,baselen);
            if(line == NULL)
            {
                free(line);
                exit(1);
            }
        }
    }

    line[index] = '\0'; // trans raw line's \n to \0 , while loop index++ already

    if(ch == -1)  // end of file
    {
        return NULL;
    }
    return line;
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fprintf(stderr,"usage: %s <text file>[.gz]\n",argv[0]);
        exit(1);
    }

    gzFile fp=gzopen(argv[1],"rb");
    if(!fp) exit(1);

    for(char *line; line=readlineGZ(fp),line; )
    {
        fprintf(stdout,"%s\n",line);
        free(line);
    }

    exit(0);
}
