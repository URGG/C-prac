

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void getLine(char* buf, int len) {
    int count = 0;
    while (count < len-1 && !feof(stdin)) {
        char ch = getc(stdin);
        if (ch == '\n') { break; }
        buf[count++] = ch;
    }
    buf[count] = '\0'; // end-of-string
}


// Generate a hash value in the range [0, 2^16)
static unsigned short hashVal;

void init() { hashVal = 0xffff; }

unsigned short doHash(char* input) {
    for(int i = strlen(input) - 1; i >= 0; --i) {
        hashVal *= 991; // a big prime
        hashVal ^= input[i];
    }
}

unsigned short theHash() { return hashVal; }



int main() {
    char theWord[64];

    getLine(theWord, 64);
    while (!feof(stdin)) {
        init();
        doHash(theWord);
        printf("%s %d\n", theWord, theHash());

        getLine(theWord, 64);
    }
}