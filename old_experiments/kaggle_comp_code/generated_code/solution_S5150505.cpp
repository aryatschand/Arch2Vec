
    #include<iostream>
    #include<stdio.h>
    #include<algorithm>
    #include<math.h>
    #include<string>
    using namespace std;
    
    
    typedef struct _node{
    	char op;
    	int val;
    	int hint;
    }node;
    
    
    node arr[100010];
    int segarr[400100];
    		
    
    bool ascendingSort(node lhs, node rhs) { return lhs.val < rhs.val; }
    
    
    void update(int left,int right,int l,int r,int idx)
    {
    	if(left > right)
    		return;
    	else if(l>=left && r<=right)
    		segarr[idx]++;
    	else if(l > right || r < left)
    		return;
    	else
    	{
    		int mid  = (l+r)>>1;
    		update(left,right,l,mid,2*idx);
    		update(left,right,mid+1,r,2*idx+1);
    	}
    }
    int query(int idx,int b,int e,int start,int end)
    {
    	if(start == end && start >= b && end <= e)
    		return segarr[idx];
    	else if(start > e || end < b)
    		return 0;
    	else
    	{
    		int mid=(start+end)>>1;
    		segarr[2*idx] += segarr[idx];
    		segarr[2*idx+1] += segarr[idx];
    		segarr[idx] = 0;
    		int v1 = query(2*idx,b,e,start,mid);
    		int v2 = query(2*idx+1,b,e,mid+1,end);
    		return max(v1,v2);
    	}
    }
    
    
    int main()
    {
    	int t;
    	scanf("%d",&t);
    	int count;
    	while(t--)
    	{
    		int k;
    		cin >> k;
    		count=0;
    		int i,j;
    		char waste;
    		int mini = 1000000009;
    		int maxi = -1;
    		for(i=0;i<k;i++)
    		{
    			char a[5],c[5];
    			int b;
    			
    			cin >> a >> b >> c;
    			
    			arr[i].op=a[0];
    			arr[i].val=b;
    			mini = min(mini,b);
    			maxi = max(maxi,b);
    			if(c[0]=='Y')
    				arr[i].hint=1;
    			else
    				arr[i].hint=0;
    		}
    		
    		sort(arr,arr+k,ascendingSort);
    		
    		count=3;
    		int last_val=arr[0].val;
    		arr[0].val=2;
    		
    		for(i=1;i<k;i++)
    		{
    			if(arr[i].val!=last_val)
    			{
    				last_val=arr[i].val;
    				arr[i].val= count ;
    				count++;
    			}
    			else
    			{
    				arr[i].val=arr[i-1].val;
    			}
    		}
    		count = count + 10;
    		
    		for(i=0;i<4*count;i++)
    			segarr[i]=0;
    		
    		for(i=0;i<k;i++)
    		{
    		
    			if(arr[i].op=='<')
    			{
    				if(arr[i].hint==1)
    				{
    					update(0,arr[i].val-1,0,count,1);
    				}
    				else
    				{
    					update(arr[i].val,count,0,count,1);	
    				}
    			}
    			else if(arr[i].op=='>')
    			{
    				if(arr[i].hint==1)
    				{
    					update(arr[i].val+1,count,0,count,1);
    				}
    				else
    				{
    					update(0,arr[i].val,0,count,1);	
    				}
    			}
    			else
    			{
    				if(arr[i].hint==1)
    				{
    					
    					update(arr[i].val,arr[i].val,0,count,1);
    				}
    				else
    				{
    					update(0,arr[i].val-1,0,count,1);
    					update(arr[i].val+1,count,0,count,1);
    				}
    			}
    		}
    	
    		int m = -1;
    		
    		int m1 = query(1,2,count-11,0,count);
    		int m2 = query(1,0,       1,0,count);
    		int m3 = query(1,count-10,count,0,count);
    		m = m1;
    		
    		if(mini > 1)
    		{
    			m = max(m,m2);
    			//printf("%d\n",k-m);
    		}
    		if(maxi < 1000000000)
    			m = max(m,m3);
    			
    		printf("%d\n",k-m);
    	
    	}
    	return 0;	
    }

