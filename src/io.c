#include <stdlib.h>
#include <stdio.h>

#include "io.h"

char *get_file(const char *file_path)
{
    char *buffer = 0;
    long length;

    FILE *f = fopen(file_path, "rb");
    if(f==NULL)
    {
        fprintf(stderr, "Error reading %s\n", file_path);
        exit(EXIT_FAILURE);
    }

    fseek(f, 0, SEEK_END);
    length = ftell(f);
    fseek(f, 0, SEEK_SET);

    buffer = calloc(length, length);
    
    if(buffer != NULL)
        fread(buffer, 1, length, f); 

    fclose(f);
    return buffer;
}