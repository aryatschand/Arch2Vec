#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Linked list node
typedef struct Node {
    int value;
    int count;
    struct Node *next;
} Node;

Node* insert(Node *head, int value) {
    Node *curr = head;
    while (curr) {
        if (curr->value == value) {
            curr->count++;
            return head;
        }
        curr = curr->next;
    }
    Node *new_node = (Node*)malloc(sizeof(Node));
    new_node->value = value;
    new_node->count = 1;
    new_node->next = head;
    return new_node;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <ARRAY_SIZE> <REPETITIONS> <RANGE>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    int REPETITIONS = atoi(argv[2]);
    int RANGE = atoi(argv[3]);

    int *arr = malloc(N * sizeof(int));
    srand(time(NULL));

    for (int rep = 0; rep < REPETITIONS; rep++) {
        for (int i = 0; i < N; i++) {
            arr[i] = rand() % RANGE;
        }
    }

    free(arr);
    return 0;
}
