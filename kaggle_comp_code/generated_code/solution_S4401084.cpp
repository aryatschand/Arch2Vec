
    #include <cstdlib>  
    #include <cctype>  
    #include <cstring>  
    #include <cstdio>  
    #include <cmath>  
    #include <algorithm>  
    #include <vector>  
    #include <string>  
    #include <iostream>  
    #include <sstream>  
    #include <map>  
    #include <set>  
    #include <queue>  
    #include <stack>  
    #include <fstream>  
    #include <numeric>  
    #include <iomanip>  
    #include <bitset>  
    #include <list>  
    #include <stdexcept>  
    #include <functional>  
    #include <utility>  
    #include <ctime>
    using namespace std;
    typedef long long LL;
    typedef unsigned long long ULL;
    #define MAX(a,b) ((a) > (b) ? (a) : (b))
    #define MIN(a,b) ((a) < (b) ? (a) : (b))
    #define MEM(a,b) memset((a),(b),sizeof(a))
    const LL INF = 1000000007;
    map<int,int> mm;
    void add(int x, int y)
    {
    	if(x > INF-7) return;
    	if(y == 1) return;
    	mm[x]++;
    	mm[y]--;
    }
    int main()
    {
    	//freopen("input.txt", "r", stdin);
    	int ncase;
    	scanf("%d",&ncase);
    	char ch[3];
    	int x;
    	char str[10];
    	while(ncase--)
    	{
    		int n;
    		scanf("%d",&n);
    		mm.clear();
    		for(int i = 0; i < n; i++)
    		{
    			scanf("%s %d %s",ch,&x,str);
    			if(ch[0] == '=')
    			{
    				if(str[0] == 'Y') add(x,x+1);
    				else add(1,x-1),add(x+1,INF);
    			}
    			if(ch[0] == '<')
    			{
    				if(str[0] == 'Y') add(1,x);
    				else add(x,INF);
    			}
    			if(ch[0] == '>')
    			{
    				if(str[0] == 'Y') add(x+1,INF);
    				else add(1,x+1);
    			}
    		}
    		int ans = 0;
    		int maxv = 0;
    		for(auto it = mm.begin(); it != mm.end(); it++)
    		{
    			ans += it->second;
    			maxv = max(maxv,ans);
    		}
    		printf("%d\n",n-maxv);
    	}
    	return 0;
    }

