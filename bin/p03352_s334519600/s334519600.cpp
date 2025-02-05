#include<iostream>

#include<vector>

#include<algorithm>

#include<cmath>

using namespace std;





int main() {



	int n;

	cin >> n;



	int ans = 1;



	for (int i = 2; i <= n; i++) {



		int num = i;

		if (i*i > n)

		{

			break;

		}

		while (num * i <= n) {

			num *= i;

		}

		ans = max(ans, num);

	}

	cout << ans << endl;

	return 0;

}