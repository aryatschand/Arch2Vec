#include <stdio.h>
#include <stdlib.h>

// Node definition
typedef struct Node {
    int value;
    struct Node* next;
} Node;

// Build a linked list from the static array
Node* build_list(void) {
    int data[16] = {
        12, 99, 3, 45, 67, 123, 999, 231,
        42, 11, 56, 1023, 54, 765, 23, 1
    };
    Node *head = NULL, *tail = NULL;
    for (int i = 0; i < 16; i++) {
        Node* newNode = (Node*)malloc(sizeof(Node));
        newNode->value = data[i];
        newNode->next  = NULL;
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    return head;
}

int main(void) {
    Node* head = build_list();
    
    long long sum = 0;
    Node* current = head;
    while (current) {
        long long val = current->value;
        sum += val * val;
        current = current->next;
    }

    printf("%lld\n", sum);  // Expect 2723180

    // Free the list
    current = head;
    while (current) {
        Node* tmp = current;
        current = current->next;
        free(tmp);
    }
    return 0;
}
