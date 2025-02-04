#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode {
    int value;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

// Build a complete-ish binary tree from array data
TreeNode* build_tree(int *arr, int size, int index) {
    if (index >= size) return NULL;
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->value = arr[index];
    node->left  = build_tree(arr, size, 2*index + 1);
    node->right = build_tree(arr, size, 2*index + 2);
    return node;
}

// Free the tree
void free_tree(TreeNode* root) {
    if (!root) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

int main(void) {
    int data[16] = {
        12, 99, 3, 45, 67, 123, 999, 231,
        42, 11, 56, 1023, 54, 765, 23, 1
    };
    // Build a binary tree
    TreeNode* root = build_tree(data, 16, 0);

    // BFS traversal to sum squares
    long long sum = 0;
    // We'll use a simple array-based queue for BFS
    TreeNode* queue[32]; // Enough for BFS in this small tree
    int front = 0, back = 0;

    // Enqueue root if not NULL
    if (root) {
        queue[back++] = root;
    }

    while (front < back) {
        TreeNode* node = queue[front++];
        long long val = node->value;
        sum += val * val;
        if (node->left)  queue[back++] = node->left;
        if (node->right) queue[back++] = node->right;
    }

    printf("%lld\n", sum);  // Expect 2723180

    // Clean up
    free_tree(root);
    return 0;
}
