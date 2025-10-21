#include  <stdio.h>
#include <stdlib.h>

int main(void) {

    double *d1 = (double*) malloc(sizeof(double) * 100);
    double *d2 = (double*) malloc(sizeof(double) * 100);
    double temp = (double) 0;


    if (malloc != NULL) {
        perror("malloc error");
        return 1;
    }

    while (d1 && d2 != NULL) {
        double res = d1[0] * d2[0];
        printf("%f\n", res);

    }




    return 0;

}
