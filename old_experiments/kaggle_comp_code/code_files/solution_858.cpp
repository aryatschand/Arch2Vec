
    #include<iostream>
    #include<string.h>
    #include<algorithm>
    #include<vector>
    #include<map>
    #include<assert.h>
    #define min 1
    #define max 1000000000
    using namespace std;
    int main()
    {
    	int t;
    	cin>>t;
    	map<int,int>::iterator i;
    //	assert(t>=0);
    	int k,count,matches,temp;
    	char op; int n; string val;
    	while(t--)
    	{
    		cin>>k;
    		temp=k;
    	//	assert(k>=1 && k<=100000);
    	//	interval.clear();
    		map<int,int> interval;
    		while(k--)
    		{
    			
    			cin>>op>>n>>val;
    		//	assert(n>=min && n<=max);
    		//	assert(val=="Yes"||val=="No");
    		//	assert(op=='>' || op=='=' || op=='<');
    		
    			if(op=='>')
    			{
    				if(val=="Yes")
    				{
    					if(n<max)
    					interval[n+1]++;
    					
    				}
    				else{
    					if(n<max)
    					interval[n+1]--;
    					interval[min]++;
    				
    				}
    
    			}
    			else if(op=='<')
    			{
    				if(val=="Yes")
    				{
    					if(n>1){
    					interval[min]++;
    					interval[n]--;
    					}
    				}
    				else{
    					interval[n]++;
    				
    				}
    			}
    			else
    			{
    				if(val=="Yes")
    				{
    					interval[n]++;
    					if(n<max)
    					interval[n+1]--;
    				}
    				else{
    					interval[min]++;
    					interval[n]--;
    					if(n<max)
    					interval[n+1]++;
    				}
    			}		
    			
    		}
    		count=0; matches=0;
    
    		for(i=interval.begin(); i!=interval.end(); i++)
    		{
    			count+=i->second;
    			
    			if(count>matches)
    				matches=count;
    		}
    		
    		cout<<(temp-matches)<<endl;
    
    	}
    	return 0;
    }
    

