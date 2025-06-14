
    #include <cstdio>
    #include <cstring>
    #include <cstdlib>
    #include <vector>
    #include <set>
    
    using namespace std ;
    
    const int MAXN = 600 + 1;
    
    int n, g[MAXN];
    vector<int> G[MAXN]; 
    int Set[MAXN];
    int Free[MAXN], child[MAXN];
    int S[MAXN][MAXN], Sz[MAXN];
    
    void solve(int node, int parent) {
    	int i, j, k, u, w, sz = 0 ;
     
    	for (i = 0; i < (int)G[node].size(); ++i) 
    		if (G[node][i] != parent)
    			solve(G[node][i], node);             
    
    	for (i = 0; i < n; ++i) Free[i] = 1;
    	for (i = 0; i < (int)G[node].size(); ++i) 
    		if (G[node][i] != parent)
    			child[sz++] = G[node][i];
     
    	for (i = 0; i < sz; ++i) S[child[i]][Sz[child[i]]++] = 0 ;
    
    	for (u = i = 0; i < sz; ++i) u = max(u, S[child[i]][0]);
     	
    	int freq[MAXN] = {};
     	for (i = 0; i < sz; ++i)
    	  	for (j = 0; j < Sz[child[i]]; ++j)
       			++freq[S[child[i]][j]];
     
     	int assigned = 0;
    	while (assigned < sz) {
    		if (freq[u] == 1 && u != 0) { 
    			Free[u] = false;
    			while (!freq[--u]);
    	  	}
      		else {
    			for (w = u + 1; !Free[w]; ++w);
    
    			int Ptr[MAXN];
    			for (j = 0; j < sz; ++j)
    				for (k = 0; k < Sz[child[j]]; ++k)
    					if (S[child[j]][k] <= w) { 
    						Ptr[child[j]] = k;
    						break;
       					}
    
    			while (1) {
    				int Mx = 0 ;
        			for (j = 0; j < sz; ++j) 
    					if (Ptr[child[j]] < Sz[child[j]])
    						Mx = max(Mx, S[child[j]][Ptr[child[j]]]);
    						
    				int tot = 0, K = -1;
    			    for (j = 0; j < sz; ++j) 
    					if (Ptr[child[j]] < Sz[child[j]]) {
    						if (S[child[j]][Ptr[child[j]]] == Mx) {
    					    	++Ptr[child[j]];
    							++tot;
    							K = j;
    					  		if (Mx == 0) break;
    						}
         					else 
    						 	Ptr[child[j]] = Sz[child[j]];
    					}
    					
    				    if (tot == 1) {
    						j = K;
    				     	break;
    				    }
       			}
    
       			Free[w] = false;
    			if (g[child[j]]) Free[g[child[j]]] = true;
    			else 
    				++assigned;
    				
    			g[child[j]] = w ;
    
       			for (i = 0; i < Sz[child[j]]; ++i)
    				if (S[child[j]][i] > u && S[child[j]][i] < w)
    					Free[S[child[j]][i]] = true;
    
    			int ptr = Sz[child[j]] - 1;
    			while (ptr >= 0 && S[child[j]][ptr] < w) 
    				--freq[S[child[j]][ptr--]];
    			Sz[child[j]] = ptr + 1;
    		}
    	}
     
     	int mx = 0;
    	for (i = 0; i < sz; ++i)
    		for (j = 0; j < Sz[child[i]]; ++j)
    			if (S[child[i]][j] > g[child[i]]) {
    				Set[S[child[i]][j]] = true;
    				mx = max(mx, S[child[i]][j]);
    			}
    			
    	for (i = 0; i < sz; ++i) {
    		Set[g[child[i]]] = true;
    		mx = max(mx, g[child[i]]);
    	}
    	
    	for (i = mx; i >= 0; --i) 
    		if (Set[i]) {
    			S[node][Sz[node]++] = i;
    			Set[i] = false;
    		}
    }
    
    int solve() {
    	memset(Sz ,0, sizeof(Sz));
    	memset(g, 0, sizeof(g));
    	int k = rand() % n;
     	solve(k, -1) ;
    	int ret = S[k][0];
    	return ret ;
    }
    
    int main() {
    	
     	int k, cases;
     
     	for (scanf("%d", &cases); cases--; ) {
    		for (int i = 0; i < MAXN; ++i) G[i].clear();
      
    		scanf("%d", &n);
    		for (int i = 1; i < n; ++i) {
    			scanf("%d", &k) ;
    			G[i].push_back(k);
    			G[k].push_back(i);
    		}
      
    		int ret = solve();
    		printf("%d\n", ret);
    		for (int i = 0; i < MAXN; ++i) G[i].clear();
    	}
    	
    	return 0;
    }
    
    

