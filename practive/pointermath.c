#include <stdio.h>


struct Person {
    char name[64];
    int age;

};

int main(int argc, char **argv) {

    struct Person people[100];

    struct Person *ptr = &people;



    for (int i = 0; i < 100; i++) {
        ptr  -> age = 0;
    }

    return 0;


}


