
    #include<ext/pb_ds/assoc_container.hpp>
    #include<ext/pb_ds/tree_policy.hpp>
    #include<bits/stdc++.h>
    using namespace __gnu_pbds;
    using namespace std;
    typedef
    tree< pair<int,int>, null_type, less< pair<int,int> >, rb_tree_tag, tree_order_statistics_node_update >
    ordered_set;
    #define lli long long int
    
    bool valid( int x ) { return x>=1 && x<=1e9; }
    
    int main()
    {
    	int t; cin>>t;
    	while ( t-- )
    	{
    		int n; cin>>n;
    		ordered_set lessNo , lessYes , moreNo , moreYes;
    		map<int,int> equalNo , equalYes; int equalYesCount = 0;
    		vector<int> check;
    		
    		for ( int i=1 ; i<=n ; i++ )
    		{
    			char c; int x; string s;
    			cin>>c>>x>>s;
    			
    			if ( c == '<' )
    			{
    				if ( s[0] == 'N' ) lessNo.insert( make_pair( x , i ) );
    				else lessYes.insert( make_pair( x , i ) );
    			}
    			else if ( c == '>' )
    			{
    				if ( s[0] == 'N' ) moreNo.insert( make_pair( x , i ) );
    				else moreYes.insert( make_pair( x , i ) );
    			}
    			else
    			{
    				if ( s[0] == 'N' ) equalNo[x]++;
    				else { equalYes[x]++; equalYesCount++; }
    			}
    			
    			check.push_back( x );
    			if ( valid( x - 1 ) ) check.push_back( x-1 );
    			if ( valid( x + 1 ) ) check.push_back( x+1 );
    		}
    		
    		int lessNoCount = lessNo.size();
    		int lessYesCount = lessYes.size();
    		int moreNoCount = moreNo.size();
    		int moreYesCount = moreYes.size();
    		
    		int ans = 1e9 + 5;
    		int vs = check.size();
    		for ( int i=0 ; i<vs ; i++ )
    		{
    			int x = check[i];
    			int thiz = 0;
    			
    			// cout<<x<<" : ";
    			// eq
    			thiz += (equalYesCount - equalYes[x]) + (equalNo[x]);
    			// cout<<"  "<<thiz;
    			
    			// less
    			thiz += lessNoCount - lessNo.order_of_key( make_pair( x , 2e9 ) );
    			// cout<<"  "<<thiz;
    			thiz += lessYes.order_of_key( make_pair( x , 2e9 ) );
    			// cout<<"  "<<thiz;
    			
    			// more
    			thiz += moreNo.order_of_key( make_pair( x , -1 ) );
    			// cout<<"  "<<thiz;
    			thiz += moreYesCount - moreYes.order_of_key( make_pair( x , -1 ) );
    			// cout<<"  "<<thiz;
    			
    			// relax
    			ans = min( ans , thiz );
    			// cout<<"\n";
    		}
    		
    		cout<<ans<<"\n";
    	}
    	
    	return 0;
    }
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     

