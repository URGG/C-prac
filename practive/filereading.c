#include <stdio.h>
#include <stdlib.h>



void file_reading(const char, *filename) {
    FILE *file = fopen(*filename, "r");
    if (file == NULL) {
        perror("could not open file");
        return;
    }

    int count = 0;
    int ch;

    while ((ch = fgetc(file)) != EOF) {
        if (ch >= 'A' && ch <= 'Z') {
            count++;
        }

    }

    fclose(file);
    printf("The total number of characters in the file is %i\n", count);


}

int main() {
    void file_reading( const char, const char *);
    file_reading( "file_reading.txt", "file_reading.txt");
    return 0;

}