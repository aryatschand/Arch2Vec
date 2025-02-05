#include <stdio.h>

int main(void) {
    // Hardcoded data in a global array
    static int data[16] = {
        12, 99, 3, 45, 67, 123, 999, 231,
        42, 11, 56, 1023, 54, 765, 23, 1
    };

    long long sum = 0;  // 64-bit accumulator
    for (int i = 0; i < 16; i++) {
        sum += (long long)data[i] * (long long)data[i];
    }

    printf("%lld\n", sum);  // Expect 2723180
    return 0;
}
