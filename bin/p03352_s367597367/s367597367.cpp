#include <iostream>

#include <algorithm>

#include <math.h>

using namespace std;

bool isPrime(int n)

{

	if (n < 2)return false;

	for (int i = 2; i < n; i++)

	{

		if (n % i == 0)return false;

	}

}

int gcd(int a, int b)

{

	for (int i = min(a, b); i >= 2; i--)

	{

		if (a % i == 0 && b % i == 0) return i;

	}

	return 1;

}

int main()

{

	int X;

	cin >> X;

	int ans = 1;

	for (int b = X; b > 1; b--)

	{

		for (int p = 2; p <= X; p++)

		{

			int t = pow(b, p);

			if (t <= X)

			{

				ans = max(t, ans);

			}

		}

	}

	cout << ans << endl;

	return 0;

}