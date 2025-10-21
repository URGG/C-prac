#include <stdio.h>
#include <stdlib.h>


int is_palindrome(char *str) {
if (str == NULL)  return 1;

    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    if (len ==0) return 1;

    char *left = str;
    char *right = str + len -1;

    while (left < right) {
        if (*left != *right) {
            return 0;
        }
        left++;
        right--;
    }

    return 1;



}

int main(void) {
    printf("\n");
    printf("%d\n", is_palindrome("Hello World!"));

    char *palindrome1 = "racecar";
    char *palindrome2 = "Hello";

    printf("'%s' is palindrome: %s\n", palindrome1, is_palindrome(palindrome1) ? "Yes" : "No");
    printf("'%s' is palindrome: %s\n", palindrome2, is_palindrome(palindrome2) ? "Yes" : "No");

    return 0;
}