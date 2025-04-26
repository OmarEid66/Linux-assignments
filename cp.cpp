#include <stdio.h>
#include <stdlib.h>
int cp_main(int argc, char* argv[])
{
    // Write your code here
    // Do not write a main() function. Instead, deal with cp_main() as the main function of your program.

    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <source_file> <destination_file>\n", argv[0]);
        return 1;
    }

    FILE* src = fopen(argv[1], "rb");
    if (src == NULL)
    {
        perror("Error opening source file");
        return 1;
    }

    FILE* dest = fopen(argv[2], "wb");
    if (dest == NULL)
    {
        perror("Error opening destination file");
        fclose(src);
        return 1;
    }

    char buffer[4096];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0)
    {
        fwrite(buffer, 1, bytes, dest);
    }

    fclose(src);
    fclose(dest);

    return 0;
}

