#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

    char buffer [1024];
    fgets(buffer, 1024, stdin);

    int upper_case = 0;
    int lower_case = 0;
    int digits = 0;

    for (int i=0; buffer[i] != '\0'; i++) {
        if (buffer[i] >= 'a' && buffer[i] <= 'z') {
            lower_case++;
        }
        if (buffer[i] >= 'A' && buffer[i] <= 'Z') {
            upper_case++;
        }
        if (buffer[i] >= '0' && buffer[i] <= '9') {
            digits++;
        }
    }

    printf("Upper Case: %d\n", upper_case);
    printf("Lower Case: %d\n", lower_case);
    printf("Digits: %d\n", digits);

    return 0;

}