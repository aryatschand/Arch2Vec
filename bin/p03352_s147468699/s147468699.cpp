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



int main() {

	int n; cin >> n;

	int mx = 1;

	for (int i = 2; i * i <= n; ++i) {

		int s = i;

		while (s * i <= n)

			s *= i;

		mx = max(s, mx);

	}

	cout << mx;

	return 0;

}