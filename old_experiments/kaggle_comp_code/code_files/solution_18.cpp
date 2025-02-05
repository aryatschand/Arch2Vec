
    #include <iostream>
    #include<bits/stdc++.h>
    using namespace std;
    
    int main() {
        int t;
        cin>>t;
        while(t--)
        {
            int n,m,k=0;
            cin>>n>>m;
            int a[n];
            for(int i=0;i<n;i++)
            {
                cin>>a[i];
            }
            for(int i=1;i<(1<<n);i++)
            {
                //cout<<i<<" ";
                int v=i,sum=0;
                for(int v1=0;v1<n;v1++)
                {
                    //cout<<v%2;
                    if(v%2==1)
                    sum+=a[v1];
                    v/=2;
                }
                //cout<<endl;
                if(sum==m)
                {
                    k=1;
                    break;
                }
            }
            (k==1)?cout<<"Yes\n":cout<<"No\n";
        }
    	// your code goes here
    	return 0;
    }
    

