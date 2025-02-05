#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int find_most_frequent(int arr[], int start, int end) {
    if (start == end) return arr[start];

    int mid = (start + end) / 2;
    int left = find_most_frequent(arr, start, mid);
    int right = find_most_frequent(arr, mid + 1, end);

    int left_count = 0, right_count = 0;
    for (int i = start; i <= end; i++) {
        if (arr[i] == left) left_count++;
        if (arr[i] == right) right_count++;
    }

    return (left_count >= right_count) ? left : right;
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
        find_most_frequent(arr, 0, N - 1);
    }

    free(arr);
    return 0;
}
