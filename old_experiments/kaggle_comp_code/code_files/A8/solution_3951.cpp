
    #include <cstdio>
    #include <cstring>
    #include <cstdlib>
    #include <vector>
    using namespace std ;
    #define MAXN 601
    vector<int> G[MAXN]; 
    int n,g[MAXN],Set[MAXN],Free[MAXN],child[MAXN],S[MAXN][MAXN],Sz[MAXN];
    inline int I(int &a)
    {
     register int c;
     a=0;
     do c=getchar_unlocked(); while(c<'0');
     do
     {
      a=(a<<1)+(a<<3)+c-'0';
      c=getchar_unlocked();
     }while(c >= '0');
    }
    inline void O(int a)
    {
     register char s[11];
     register int t = -1;
     do s[++t]=a%10+'0',a/=10; while(a>0);
     while(t>=0) putchar_unlocked(s[t--]);
     putchar_unlocked('\n');
    } 
    void solve(int node,int parent)
    {
    	int i, j, k, u, w, sz = 0 ;
    	for (i = 0; i < G[node].size(); ++i) if (G[node][i] != parent) solve(G[node][i], node);             
    	for (i = 0; i < n; ++i) Free[i] = 1;
    	for (i = 0; i < G[node].size(); ++i) if (G[node][i] != parent) child[sz++] = G[node][i];
    	for (i = 0; i < sz; ++i) S[child[i]][Sz[child[i]]++] = 0 ;
    	for (u = i = 0; i < sz; ++i) u = max(u, S[child[i]][0]);
    	int freq[MAXN] = {};
     	for (i = 0; i < sz; ++i) for (j = 0; j < Sz[child[i]]; ++j)	++freq[S[child[i]][j]];
     	for(int assigned = 0;assigned < sz;) if (freq[u] == 1 && u != 0) { Free[u] = 0; while (!freq[--u]); }
      		else 
      		{
    			for (w = u + 1; !Free[w]; ++w);
    			int Ptr[MAXN];
    			for (j = 0; j < sz; ++j) for (k = 0; k < Sz[child[j]]; ++k) if (S[child[j]][k] <= w) { Ptr[child[j]] = k; break; }
    			while (1)
    			{
    				int Mx = 0 ;
        			for (j=0; j<sz; ++j) if (Ptr[child[j]] < Sz[child[j]]) Mx = max(Mx, S[child[j]][Ptr[child[j]]]);
    				int tot = 0, K = -1;
    			    for (j = 0; j < sz; ++j) 
    					if (Ptr[child[j]] < Sz[child[j]])
    					{
    						if (S[child[j]][Ptr[child[j]]]==Mx) { ++Ptr[child[j]]; ++tot; K = j;if (Mx == 0) break; }
         					else Ptr[child[j]] = Sz[child[j]];
    					}
    				    if (tot == 1) {	j = K; break; }
       			}
       			Free[w] = 0;
    			if (g[child[j]]) Free[g[child[j]]] = 1;
    			else ++assigned;
    			g[child[j]] = w ;
       			for(i=0; i<Sz[child[j]]; ++i) if (S[child[j]][i]>u && S[child[j]][i]<w)	Free[S[child[j]][i]]=1;
    			int ptr = Sz[child[j]] - 1;
    			while (ptr >= 0 && S[child[j]][ptr] < w) --freq[S[child[j]][ptr--]];
    			Sz[child[j]] = ptr + 1;
    		} 
     	int mx = 0;
    	for(i=0;i<sz;++i) for(j=0;j<Sz[child[i]];++j) if(S[child[i]][j]>g[child[i]]) Set[S[child[i]][j]]=1,mx=max(mx,S[child[i]][j]);			
    	for (i = 0; i < sz; ++i) Set[g[child[i]]]=1,mx=max(mx, g[child[i]]);	
    	for (i = mx; i >= 0; --i) if (Set[i]) S[node][Sz[node]++] = i, Set[i]=0;
    }
    int solve()
    {
    	memset(Sz ,0, sizeof(Sz));
    	memset(g, 0, sizeof(g));
    	int k = rand() % n,ret;
     	solve(k, -1) ;
    	ret = S[k][0];
    	return ret ;
    }
    int main()
    {
     	int k, cases;
    	I(cases);
    	while(cases--)
    	{
    		for (int i = 0; i < MAXN; ++i) G[i].clear();
    		I(n);
    		for (int i = 1; i < n; ++i)	I(k), G[i].push_back(k), G[k].push_back(i);
      		int ret = solve();	O(ret);
    		for (int i = 0; i < MAXN; ++i) G[i].clear();
    	}
    }

