#include <stdio.h>

int main(void) {
    static int data[16] = {
        12, 99, 3, 45, 67, 123, 999, 231,
        42, 11, 56, 1023, 54, 765, 23, 1
    };

    long long sum = 0;
    for (int i = 0; i < 16; i++) {
        long long val;
        switch (i) {
            case 0:  val = data[0];  break;
            case 1:  val = data[1];  break;
            case 2:  val = data[2];  break;
            case 3:  val = data[3];  break;
            case 4:  val = data[4];  break;
            case 5:  val = data[5];  break;
            case 6:  val = data[6];  break;
            case 7:  val = data[7];  break;
            case 8:  val = data[8];  break;
            case 9:  val = data[9];  break;
            case 10: val = data[10]; break;
            case 11: val = data[11]; break;
            case 12: val = data[12]; break;
            case 13: val = data[13]; break;
            case 14: val = data[14]; break;
            default: val = data[15]; break;
        }
        sum += val * val;
    }

    printf("%lld\n", sum);  // Expect 2723180
    return 0;
}
