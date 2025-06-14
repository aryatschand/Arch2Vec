
    #include<iostream>
    #include<cstdio>
    #include<set>
    
    using namespace std;
    
    #define gc getchar_unlocked
    
    void scanint(int &x)
    {
    	register int c = gc();
    	x = 0;
    	for(;(c<48 || c>57);c = gc());
    	for(;c>47 && c<58;c = gc()) {x = (x<<1) + (x<<3) + c - 48;}
    }
    
    char scanchar()
    {
    	int temp=getchar_unlocked();
    	while(temp == ' ' || temp == '\n')
    		temp=getchar_unlocked();
    	return (char)temp;
    }
    
    string scanstring()
    {
    	string i="";
    	int temp=getchar_unlocked();
    	while(!((temp>='a' && temp<='z') || (temp>='A' && temp <= 'Z')))
    		temp=getchar_unlocked();
    	while(((temp>='a' && temp<='z') || (temp>='A' && temp <= 'Z')))
    	{
    		i+=(char)temp;
    		temp=getchar_unlocked();
    	}
    	return i;
    }
    
    struct lim
    {
    	long long int val;
    	int count;
    	lim(long long int val_, int count_)
    	{
    		val = val_;
    		count = count_;
    	}
    };
    
    struct compare
    {
    	bool operator()(const lim a, const lim b)
    	{
    		return (a.val < b.val);
    	}
    };
    
    set<lim, compare> ranges;
    set<lim, compare>::iterator it;
    int ans;
    
    #define cons 1000000000LL
    
    void insertrange(long long int start, long long int end)
    {
    	if(end < 1 || start > cons)
    	{
    		ans++;
    		return ;
    	}
    	it = ranges.find(lim(start, 1));
    	if(it == ranges.end())
    		ranges.insert(lim(start, 1));
    	else
    	{
    		int newcount = it->count+1;
    		ranges.erase(it);
    		ranges.insert(lim(start, newcount));
    	}
    	it = ranges.find(lim(end+1, 1));
    	if(it == ranges.end())
    		ranges.insert(lim(end + 1, -1));
    	else
    	{
    		int newcount = it->count-1;
    		ranges.erase(it);
    		ranges.insert(lim(end + 1, newcount));
    	}
    	return ;
    }
    
    
    int main()
    {
    	int t;
    	scanint(t);
    	while(t--)
    	{
    		ans = 0;
    		ranges.clear();
    		int k;
    		scanint(k);
    		char ch;
    		int val;
    		string str;
    		for(int i=0; i<k; ++i)
    		{
    			ch = scanchar();
    			scanint(val);
    			str = scanstring();
    			if(ch == '<')
    			{
    				if(str == "Yes")
    					insertrange(1, val - 1);
    				else
    					insertrange(val, cons);
    			}
    			else if(ch == '>')
    			{
    				if(str == "Yes")
    					insertrange(val + 1, cons);
    				else
    					insertrange(1, val);
    			}
    			else
    			{
    				if(str == "Yes")
    					insertrange(val, val);
    				else
    				{
    					insertrange(val + 1, cons);
    					insertrange(1, val - 1);
    				}
    			}
    		}
    		int max = 0;
    		int currval = 0;
    		for(it = ranges.begin(); it != ranges.end(); ++it)
    		{
    			currval += it->count;
    			if(currval > max)
    				max = currval;
    		}
    		cout<<k - max<<endl;
    	}	
    	return 0;
    }
    		
    

