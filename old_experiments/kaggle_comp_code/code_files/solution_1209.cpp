
    #define _USE_MATH_DEFINES
    #define _CRT_SECURE_NO_DEPRECATE
    #include <iostream>
    #include <cstdio>
    #include <cstdlib>
    #include <vector>
    #include <sstream>
    #include <string>
    #include <map>
    #include <set>
    #include <algorithm>
    #include <iomanip>
    #include <functional>
    #include <bitset>
    #include <cassert>
    #include <cmath>
    #include <ctime>
    #include <queue>
    #include <list>
    #include <memory.h>
    #include <complex>
    #include <numeric>
    using namespace std;
    #pragma comment(linker, "/STACK:256000000")
    #define mp make_pair
    #define pb push_back
    #define all(C) (C).begin(), (C).end()
    #define sz(C) (int)(C).size()
    #define PRIME 1103
    #define PRIME1 31415
    #define INF ((1ll << 30) - 1)
    #define MOD 99990001
    #define FAIL ++*(int*)0
    #define EPS 1e-6
    template<class T> T sqr(T a) {return a * a;}
    typedef long long int64;
    typedef unsigned long long uint64;
    typedef pair<int, int> pii;
    typedef pair<int64, int64> pi64;
    typedef pair<int, pii> piii;
    typedef vector<int> vi;
    typedef vector<int64> vi64;
    typedef vector<vi64> vvi64;
    typedef vector<pi64> vpi64;
    typedef vector<vpi64 > vvpi64;
    typedef vector<pii> vpii;
    typedef vector<vector<int> > vvi;
    typedef vector<vvi> vvvi;
    typedef vector<vector<pair<int, int > > > vvpii;
    typedef vector<vector<vector<pair<int, int > > > > vvvpii;
    typedef complex<long double> cd;
    //----------------------------------------------------------    
    int n, q, nu = 1, k;
    int bonus = 0;
    struct nod
    {
    	int x, v, c;
    	void in (int a, int b, int k)
    	{
    		x = a, v = b, c = k;
    	}
    	const bool operator < (nod a) const 
    	{
    		if (x < a.x)
    			return true;
    		return false;
    	}
    };
    nod ot[1000000];
    void init()
    {
    	cin >> n;
    	bonus = 0;
    	for (int i = 0; i < n; ++i)
    	{
    		char s;
    		string st;
    		int a;
    		cin >> s >> a >> st;
    		if (st[0] == 'Y')
    		{
    			if (s == '=')
    				ot[i].in(a, 1, 0);
    			if (s == '>' )
    				ot[i].in(a + 1, 0, 1);
    			if (s == '<' && a != 1)
    			{
    				bonus++;
    				ot[i].in(a, 0, -1);
    			}
    		}
    		else
    		{
    			if (s == '=')
    			{
    				bonus++;
    				ot[i].in(a, -1, 0);
    			}
    			if (s == '>')
    			{
    				bonus++;
    				ot[i].in(a + 1, 0, -1);
    			}
    			if (s == '<')
    				ot[i].in(a, 0, 1);
    		}
    	}
    	ot[n].in(1e9 + 1, -1e9, -1e9);
    	ot[n + 1].in(0, -1e9, 0); 
    	sort(ot, ot + n + 2);
    }
    void solve()
    {
    	int o = bonus;
    	int v = 0;
    	for (int i = 0; i < n + 2; ++i)
    	{
    		v += ot[i].v;
    		bonus += ot[i].c;
    		if (i != n + 1)
    			if (ot[i + 1].x == ot[i].x)
    				continue;
    		o = max(bonus + v, o);
    		v = 0;
    	}
    	printf("%d\n", n - o);
    }
    int main()
    {
        #ifndef ONLINE_JUDGE
            freopen ("input.txt", "r", stdin); freopen ("output.txt", "w", stdout);
        #endif
    	int t;
    	cin >> t;
    	for (int i = 0; i < t; ++i)
    	{ 
    		init();
    		solve();
    		memset(ot, 0, sizeof(ot));
    	}
        return 0;
    }

