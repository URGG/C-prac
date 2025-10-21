#include <stdlib.h>
#include <stdio.h>

struct ListNode {
    char data;
    struct ListNode* next;
};

struct List {
    struct ListNode* head;
};

void init(struct List* l) {
    l -> head = (struct ListNode* )0;
}

void insert(struct List* l, char value) {
    //save the old node
    struct ListNode* oldhead = l -> head;
    //make the new node
    struct ListNode* newNode = malloc(sizeof(struct ListNode));
    newNode -> data = value;
    newNode -> next = oldhead;

    //list head points to the new node
    l -> head = newNode;

}


void print_helper(struct ListNode* node) {
    if (node == 0) {
        return;
    }
    printf("%c", node -> data);
    print_helper(node -> next);
}

void printlist(struct List* l) {
    print_helper(l -> head);

}

int main() {
    char* text= "once upon a time";
    struct list* l = malloc(sizeof(struct List));
    init(&l);
    while (*text != 0) {
        insert(&l, *text++);
    }
    printlist(l);
    return 0;
}