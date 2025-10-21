#include <stdio.h>
#include <stdlib.h>


long long fact(long longn) {
    //base case
    if (n < 2) {
        return 1;
    }
    return n * fact(n-1);
}

int main(void) {
    printf("%lld\n", fact(20));

    return 0;
}