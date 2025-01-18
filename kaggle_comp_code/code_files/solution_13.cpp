
    //basma taala
    #include<bits/stdc++.h>
    using namespace std;
    int a[20];
    
    int call(int i, int n, int x)
    {
    	for(int t = i; t < n; t++)
    	{
    		if(a[t] > x)
    		break;
    		
    		else if(a[t] == x)
    		{
    			return 1;
    		}
    		else
    		{
    			if(call(t + 1, n, x - a[t]))
    			{
    				return 1;
    			}
    		}
    	}
    	return 0;
    }
    
    int main()
    {
    	int t;
    	cin >> t;
    	while(t--)
    	{
    		int n;
    		cin >> n;
    		
    		int m;
    		cin >> m;
    		
    		for(int i = 0; i < n; i++)
    		cin >> a[i];
    		
    		sort(a, a + n);
    		
    		if(call(0, n, m))
    		{
    			cout << "Yes" << endl;
    		}
    		else
    		{
    			cout << "No" << endl;
    		}
    	}
    }

