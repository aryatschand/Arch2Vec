#include <stdio.h>

// Data stored globally
static int data[16] = {
    12, 99, 3, 45, 67, 123, 999, 231,
    42, 11, 56, 1023, 54, 765, 23, 1
};

// We'll define 16 different functions, each returns data[i]^2:
long long f0(void)  { long long v=data[0];  return v*v; }
long long f1(void)  { long long v=data[1];  return v*v; }
long long f2(void)  { long long v=data[2];  return v*v; }
long long f3(void)  { long long v=data[3];  return v*v; }
long long f4(void)  { long long v=data[4];  return v*v; }
long long f5(void)  { long long v=data[5];  return v*v; }
long long f6(void)  { long long v=data[6];  return v*v; }
long long f7(void)  { long long v=data[7];  return v*v; }
long long f8(void)  { long long v=data[8];  return v*v; }
long long f9(void)  { long long v=data[9];  return v*v; }
long long f10(void) { long long v=data[10]; return v*v; }
long long f11(void) { long long v=data[11]; return v*v; }
long long f12(void) { long long v=data[12]; return v*v; }
long long f13(void) { long long v=data[13]; return v*v; }
long long f14(void) { long long v=data[14]; return v*v; }
long long f15(void) { long long v=data[15]; return v*v; }

typedef long long (*func_t)(void);

int main(void) {
    // Build an array of 16 function pointers
    func_t funcs[16] = {
        f0, f1, f2, f3,
        f4, f5, f6, f7,
        f8, f9, f10, f11,
        f12, f13, f14, f15
    };

    long long sum = 0;
    for (int i = 0; i < 16; i++) {
        sum += funcs[i]();
    }

    printf("%lld\n", sum);  // Expect 2723180
    return 0;
}
