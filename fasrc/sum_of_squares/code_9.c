#include <stdio.h>

int main(void) {
    static int data[16] = {
        12, 99, 3, 45, 67, 123, 999, 231,
        42, 11, 56, 1023, 54, 765, 23, 1
    };

    // A permuted index array (16 distinct indices in random order):
    static int indexOrder[16] = { 5, 14, 1, 10, 9, 0, 12, 7,
                                  15, 6, 13, 2, 11, 3, 8, 4 };

    long long sum = 0;
    for (int i = 0; i < 16; i++) {
        int idx = indexOrder[i];
        long long val = data[idx];
        sum += val * val;
    }

    printf("%lld\n", sum); // Expect 2723180
    return 0;
}
