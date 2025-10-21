#include <stdio.h>
#include <stdlib.h>

//Define the node strucure
struct node {
    int value;
    struct Node *next;
}Node;
//creating a new node
Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizof(Node));
    if (newNode == NULL) {
        exit(1);
    }
    newNode -> value = data;
    newNode -> next = NULL;
    return newNode;
}

//Insertion at the head
void Insertion_head(Node** head, int data) {
    Node* newNode = createNode(data);
    newNode -> next = *head;

}

//insertion at the tail
void insertion_tail(Node** tail, int data) {
    Node* newNode = createNode(data);

    if (*head == NULL) {
        *head = newNode;
        return;
    }

    Node* current = *head;
    while (current -> next != NULL) {
        current -> next = newNode;

    }
    current -> next = newNode;

}

void


