
    #include <iostream>
    #include <cstdio>
    #include <cassert>
    #include <map>
    
    using namespace std;
    
    const int small=1, big=1000000000;
    typedef map<int,int> R;
    R count;
    int main()
    {
    	int t;
    	cin>>t;
    	assert (t<=20);
    	while (t--!=0)
    	{
    		int k;
    		cin>>k;
    		assert (0<=k && k<=100000);
    		count.clear();
    		count[small]=0;
    		for (int i=0; i<k; i++)
    		{
    			char o[2], v[4];
    			int l;
    			scanf ("%1s%d%3s", o, &l, v);
    			assert (small<=l && l<=big);
    			if (*o=='<' && *v=='Y') // less
    			count[l]++;
    			if (*o=='<' && *v=='N') // greater than equal to
    			{
    				count[small]++;
    				count[l]--;
    			}
    			if (*o=='>' && *v=='Y') //greater
    			{
    				count[small]++;
    				count[l+1]--;
    			}
    			if (*o=='>' && *v=='N') // less than equal to
    			count[l+1]++;
    			if (*o=='=' && *v=='Y') // equal to
    			{
    				count[small]++;
    				count[l]--;
    				count[l+1]++;
    			}
    			if (*o=='=' && *v=='N') // !=
    			{
    				count[l]++;
    				count[l+1]--;
    			}
    		}
    		int ccount = 0, mcount = k;
    		for (R::iterator it = count.begin(); it!=count.end(); it++)
    		{
    			ccount += it->second;
    			if (it->first <= big) mcount = min(mcount,ccount);
    		}
    		printf ("%d\n", mcount);
    	}
    }  

