#include <stdio.h>
#include <stdlib.h>


int main() {
    char buffer[1024];
    fgets(buffer, 1024 , stdin);

    printf("%s\n", buffer);
    for (int i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] == '\n') {
            buffer[i] = '\0';
        }

    }

    int word_count = 0;
    int in_word = 0;

    for (int i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] == ' ') {
            in_word++;

        }else {
            if (buffer[i] == '\n') {
                word_count++;
            }
        }
    }

    return 0;


}