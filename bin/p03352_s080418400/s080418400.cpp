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

    vector<bool> expo(X + 1);



    expo[1] = true;

    for (int i = 2; i <= X; i++) {

        int tmp = i * i;

        while (tmp <= X) {

            expo[tmp] = true;

            tmp *= i;

        }

    }



    for (int i = X; 1 <= i; i--) {

        if (expo[i]){

            cout << i << endl;

            break;

        }

    }



    return 0;

}
