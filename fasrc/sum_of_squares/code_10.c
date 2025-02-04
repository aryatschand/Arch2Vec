#include <stdio.h>

int main(void) {
    static int data[16] = {
        12, 99, 3, 45, 67, 123, 999, 231,
        42, 11, 56, 1023, 54, 765, 23, 1
    };

    long long sum = 0;

    for (int i = 0; i < 16; i++) {
        int x = data[i];
        // Arbitrary multi-branch logic
        if (x > 500) {
            if (x > 750) {
                if (x > 900) {
                    // Very large
                    sum += (long long)x * x;
                } else {
                    // Between 750 and 900
                    sum += (long long)x * x;
                }
            } else {
                // Between 500 and 750
                sum += (long long)x * x;
            }
        } else {
            // x <= 500
            if (x > 100) {
                sum += (long long)x * x;
            } else if (x > 50) {
                sum += (long long)x * x;
            } else {
                sum += (long long)x * x;
            }
        }
    }

    printf("%lld\n", sum); // Expect 2723180
    return 0;
}
