
    using namespace std;
     
    #include <cstdio>
    #include <cassert>
    #include <map>
     
    const int MIN=1, MAX=1000000000;
     
    typedef map<int,int> mi;
     
    mi cnt;
     
    int main()
    {
    	int t; scanf ("%d", &t); assert (t<=20); while (t--)
    	{
    		int k; scanf ("%d", &k); assert (0<=k && k<=100000); 
    		cnt.clear();
    		cnt[MIN]=0;
    		for (int i=0; i<k; i++)
    		{
    			char o[2], v[4]; int l; scanf ("%1s%d%3s", o, &l, v); assert (MIN<=l && l<=MAX);
    			if (*o=='<' && *v=='Y') // <
    				cnt[l]++;
    			if (*o=='<' && *v=='N') // >=
    			{
    				cnt[MIN]++;
    				cnt[l]--;
    			}
    			if (*o=='>' && *v=='Y') // >
    			{
    				cnt[MIN]++;
    				cnt[l+1]--;
    			}
    			if (*o=='>' && *v=='N') // <=
    				cnt[l+1]++;
    			if (*o=='=' && *v=='Y') // ==
    			{
    				cnt[MIN]++;
    				cnt[l]--;
    				cnt[l+1]++;
    			}
    			if (*o=='=' && *v=='N') // !=
    			{
    				cnt[l]++;
    				cnt[l+1]--;
    			}
    		}
    		int ccnt = 0, mcnt = k;
    		for (mi::iterator it = cnt.begin(); it!=cnt.end(); it++)
    		{
    			ccnt += it->second;
    			if (it->first <= MAX) mcnt = min(mcnt,ccnt);
    		}
     
    		printf ("%d\n", mcnt);
    	}
    }

