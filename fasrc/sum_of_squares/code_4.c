#include <stdio.h>

static int data[16] = {
    12, 99, 3, 45, 67, 123, 999, 231,
    42, 11, 56, 1023, 54, 765, 23, 1
};

long long square(int x) {
    return (long long)x * x;
}

int main(void) {
    long long sum = 0;
    int *ptr = data;
    int *end = data + 16;

    while (ptr < end) {
        sum += square(*ptr);
        ptr++;
    }

    printf("%lld\n", sum);  // Expect 2723180
    return 0;
}
