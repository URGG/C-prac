#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: readingiscool <filename>\n");
        return 1;
    }
    int counter = 1;

    FILE *file;

    file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("cant open or find file ");
        exit(1);
    }
    char buffer [1024];

    while (fgets(buffer, 1024, file) != NULL) {

        printf("%d: %s", counter, buffer);
        counter++;
    }
    fclose(file);
    return 0;



}