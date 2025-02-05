#define rep(i, n) for (int i = 0; i < (n); i++)

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

using ll = long long;

using P = pair<int, int>;

const int MOD = 1000000007;



int main() {

    int X;

    cin >> X;

    int maxValue = 1;

    for (int i = 1; i <= X; i++) {  // O(N^2) -> 1000 * 1000 = 10^6

        for (int j = 2; j <= X; j++) {

            if (pow(i, j) <= X) {

                maxValue = max(maxValue, (int)pow(i, j));

            }

        }

    }



    cout << maxValue << endl;



    return 0;

}
