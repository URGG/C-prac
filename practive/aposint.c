#include <stdio.h>
#include <stdlib.h>

int main() {
    int n;
    scanf("%d", &n);
    int *arr = (int *)malloc(sizeof(int) * n);


    if (arr == NULL) {
        printf("malloc failed\n");
        return 1;
    }

    int max = arr[0];

    for (int i =0; i < n; i++) {
        scanf("%d", &arr[i]);
        if (arr[i] > max) {
            max = arr[i];
        }


    }
    printf("MAX:%d\n", max);
    free(arr);
    return 0;

}