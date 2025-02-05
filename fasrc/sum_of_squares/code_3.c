#include <stdio.h>

static int data[16] = {
    12, 99, 3, 45, 67, 123, 999, 231,
    42, 11, 56, 1023, 54, 765, 23, 1
};

// Recursive helper to sum squares from index start to end-1
long long sum_squares_recursive(int start, int end) {
    if (start >= end) {
        return 0;
    }
    if (start + 1 == end) {
        long long val = data[start];
        return val * val;
    }
    int mid = (start + end) / 2;
    long long leftSum  = sum_squares_recursive(start, mid);
    long long rightSum = sum_squares_recursive(mid, end);
    return leftSum + rightSum;
}

int main(void) {
    long long sum = sum_squares_recursive(0, 16);
    printf("%lld\n", sum);  // Expect 2723180
    return 0;
}
