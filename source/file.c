#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "file.h"

extern char *file_read(char *path)
{
    char cwd[_MAX_PATH];
    getcwd(cwd, sizeof(cwd));
    printf("CWD : %s\n", cwd);
    printf("File Path : %s\n", path);

    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        fprintf(stderr, "File opening error. Path : %s\n", path);
        return NULL;
    }

    size_t size;
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *data = calloc(size, sizeof(char));
    if (data == NULL)
    {
        fprintf(stderr, "File memory allocation error. Path : %s\n", path);
        return NULL;
    }

    fread(data, sizeof(char), size / sizeof(char), file);

    return data;
}
