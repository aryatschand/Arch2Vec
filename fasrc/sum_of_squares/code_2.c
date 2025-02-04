#include <stdio.h>

int main(void) {
    static int data[16] = {
        12, 99, 3, 45, 67, 123, 999, 231,
        42, 11, 56, 1023, 54, 765, 23, 1
    };

    long long sum = 0;

    // Manual unrolling of all 16 iterations
    sum += (long long)data[0]  * data[0];
    sum += (long long)data[1]  * data[1];
    sum += (long long)data[2]  * data[2];
    sum += (long long)data[3]  * data[3];
    sum += (long long)data[4]  * data[4];
    sum += (long long)data[5]  * data[5];
    sum += (long long)data[6]  * data[6];
    sum += (long long)data[7]  * data[7];
    sum += (long long)data[8]  * data[8];
    sum += (long long)data[9]  * data[9];
    sum += (long long)data[10] * data[10];
    sum += (long long)data[11] * data[11];
    sum += (long long)data[12] * data[12];
    sum += (long long)data[13] * data[13];
    sum += (long long)data[14] * data[14];
    sum += (long long)data[15] * data[15];

    printf("%lld\n", sum);  // Expect 2723180
    return 0;
}
