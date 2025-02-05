#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Comparison function for qsort
int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

// Function to find the most frequent element using sorting
int most_frequent_sorted(int arr[], int n) {
    qsort(arr, n, sizeof(int), compare);
    
    int max_count = 1, current_count = 1, most_frequent = arr[0];

    for (int i = 1; i < n; i++) {
        if (arr[i] == arr[i - 1]) {
            current_count++;
        } else {
            if (current_count > max_count) {
                max_count = current_count;
                most_frequent = arr[i - 1];
            }
            current_count = 1;
        }
    }
    return (current_count > max_count) ? arr[n - 1] : most_frequent;
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
        most_frequent_sorted(arr, N);
    }
    
    free(arr);
    return 0;
}
