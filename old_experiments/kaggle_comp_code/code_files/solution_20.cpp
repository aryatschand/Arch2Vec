
    #include <iostream>
    using namespace std;
    
    int A[30],N;
    
    bool subset(int i,int sum)
    {
        if(sum==0)  return 1;
        if(i==N)    return 0;
        
        if(sum-A[i] >= 0)
        {
            return ( subset(i+1,sum-A[i]) || subset(i+1,sum) );
        }
        else
        {
            return  (subset(i+1,sum));
        }
        return 0;
    }
    
    
    int main() {
    	// your code goes here
    	int T,M;
    	cin>>T;
    	while(T--)
    	{
    	    cin>>N>>M;
    	    
    	    for(int i=0;i<N;i++)    cin>>A[i];
    	    
    	    if(subset(0,M))
    	    {
    	        cout<<"Yes"<<endl;
    	    }
    	    else
    	    {
    	        cout<<"No"<<endl;
    	    }
    	    
    	}
    	return 0;
    }
    

