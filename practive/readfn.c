#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

    int num = 0;
    int sum = 0;
    int count = 0;


    scanf("%d", &num);

    while (num != -1) {
        sum = sum + num;
        count++;
        scanf("%d", &num);

    }

    if (count > 0) {
        printf("%d\n", sum);
        double average = (double) sum /count;
        printf("%.2f\n", average);

    }else {
        printf("no numbers enterd\n");
    }



}