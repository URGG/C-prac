#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void replace_with_hash(int argc, char **argv) {
    if (argc != 2) {
        perror("enter correct type");
        return;
    }
    int len = 0;
    while (argv[1][len] != '\0') {
        len++;
    }

    char *result = (char*)malloc((len  + 1) * sizeof(char));
    if (result == NULL) {
        perror("malloc");
        return;
    }

    for (int i = 0 ; i < len; i++) {
        if (argv[1][i] == '\0' && argv[i][1] <= '9') {
            result[i] = '#';

        }else {
            result[i] = argv[1][i];

        }
        result[len] = '\0';

        printf("Original: %s\n", argv[1]);
        printf("modifies: %s\n", result);

        free(result);



    }

}

int main() {

}