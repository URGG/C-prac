#include<stdio.h>
#include <stdlib.h>

struct node {
    int data;
    struct node *left;
    struct node *right;

};

struct node* createNode(int value) {
    struct node *newnode =  (struct node *)malloc(sizeof(struct node));
    newnode -> data = value;
    newnode -> left = NULL;
    newnode -> right  = NULL;
    return newnode;
}

struct node* insert(struct node* root, int value) {

    if (root == NULL) {
        return createNode(value);
    }

    if (value < root -> data) {
        root -> left = insert(root -> left, value);
    }else if (value > root -> data) {
        root -> right = insert(root -> right, value);

    }
    return root;
}

struct node* search(struct node* root, int value) {
    if (root == NULL || root -> data == value) {
        return root;
    }

    if ( value < root -> data) {
        return search(root -> left, value);

    }

    return search(root -> right, value);
}

void inorder(struct node* root) {
    if (root != NULL) {
        inorder(root -> left);
        printf("%d ", root -> data);
        inorder(root -> right);
    }
}

int main() {
    struct node *root = NULL;
    root = insert(root, 10);
    root = insert(root, 20);
    root = insert(root, 30);
    root = insert(root, 40);
    root = insert(root, 50);
    root = insert(root, 60);
    root = insert(root, 70);
    root = insert(root, 80);
    root = insert(root, 90);
    root = insert(root, 100);
    root = insert(root, 110);
    root = insert(root, 120);
    root = insert(root, 130);
    root = insert(root, 140);
    root = insert(root, 150);
    root = insert(root, 160);

    inorder(root);
    printf("\n");




}
