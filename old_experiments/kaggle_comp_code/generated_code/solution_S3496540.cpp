
    #include <cstdio>
    #include <cassert>
    #include <map>
    #include <iostream>
    #define small 1
    #define big 1000000000
    using namespace std;
    
    map<int,int> count;
    int main()
    {
    	int t;
    	scanf ("%d", &t);
    	assert (t<=20);
    	int m = 1;
    	while (m++ <= 20)
    	{
    		int k;
    		scanf ("%d", &k);
    		assert (0<=k && k<=100000);
    		count.clear();
    		count[small]=0;
    		for (int i=0; i<k; i++)
    		{
    			char op[2], value[4];
    			int li;
    			scanf ("%1s%d%3s", op, &li, value);
    			assert (small<=li && li<=big);
    			if (*op=='<' && *value=='Y') 
    				count[li]++;
    			if (*op=='<' && *value=='N')
    			{
    				count[small]++;
    				count[li]--;
    				
    			}
    			if (*op=='>' && *value=='Y') 
    			{
    				count[small]++;
    				count[li+1]--;
    			}
    			if (*op=='>' && *value=='N') 
    				count[li+1]++;
    			if (*op=='=' && *value=='Y') 
    			{
    				count[small]++;
    				count[li]--;
    				count[li+1]++;
    			}
    			if (*op=='=' && *value=='N')
    			{
    				count[li]++;
    				count[li+1]--;
    			}
    		}
    		int ccount = 0, mcount = k;
    		for (map<int,int>::iterator it = count.begin(); it!=count.end(); it++)
    		{
    			ccount += it->second;
    			if (it->first <= big) mcount = min(mcount,ccount);
    		}
    		printf ("%d\n", mcount);
    	}
    }

