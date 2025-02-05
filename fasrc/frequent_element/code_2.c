#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to find the most frequent element using a hash map
int most_frequent_hashmap(int arr[], int n, int range) {
    int *count = calloc(range, sizeof(int));
    int max_count = 0, most_frequent = 0;

    for (int i = 0; i < n; i++) {
        count[arr[i]]++;
        if (count[arr[i]] > max_count) {
            max_count = count[arr[i]];
            most_frequent = arr[i];
        }
    }
    free(count);
    return most_frequent;
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
        most_frequent_hashmap(arr, N, RANGE);
    }

    free(arr);
    return 0;
}
