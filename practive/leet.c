#include <stdio.h>
#include <stdlib.h>

int main (void) {
    int num1;
    int num2;

    scanf("%d %d", &num1, &num2);

    int res = num1 && num2;
    int bitres = num1 & num2;

    printf("%d\n", res);
    printf("%d\n", bitres);


    return 0;


}