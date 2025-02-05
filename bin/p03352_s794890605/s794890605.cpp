#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <set>
#include <map>
#include <queue>
#include <stack>
#include <bitset>
using namespace std;



using namespace std;

typedef long long ll;

typedef unsigned long long ull;



int mypow(int n, int p) {

    if (p == 0) return 1;

    else if (p & 1) {

        return n * mypow(n, p - 1);

    } else {

        int half = mypow(n, p >> 1);

        return half * half;

    }

}



int main() {

    int x;

    cin >> x;

    int ans = 1;

    for (int j = 2; j <= sqrt(x); ++j) {

        int tmp = j;

        int cnt = 0;

        while (tmp < x) {

            tmp *= j;

            ++cnt;

        }

        if (tmp == x) ++cnt;

        else tmp /= j;

        if (cnt >= 2)

            ans = max(tmp, ans);

    }

    cout << ans << endl;

    return 0;

}



static int speedUp = []() {

    ios::sync_with_stdio(false);

    cin.tie(nullptr);

    cout.tie(nullptr);

    return 0;

}();