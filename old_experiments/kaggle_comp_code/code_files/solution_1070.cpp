
    using namespace std;
    #include<cstdio>
    #include<map>
    const int small=1,big=1000000000;
    typedef map<int,int>R;
    R count;
    int main() 
    {
     int t;
     scanf("%d",&t);
     while(t--)
     {
     	int k;
     	scanf("%d",&k);
     	count.clear();
     	count[small]=0;
     	for(int i=0;i<k;i++)
     	{
     		char o[2],v[4];
     		int l;
     		scanf("%1s%d%3s",o,&l,v);
     		if(*o=='<'&&*v=='Y')
     		{
     			count[l]++;
     		}
     		if(*o=='<'&&*v=='N')
     		{
     			count[small]++;
     			count[l]--;
     		}
     		if(*o=='>'&&*v=='Y')
     		{
     			count[small]++;
     			count[l+1]--;
     		}
     		if(*o=='>'&&*v=='N')
     		{
     			count[l+1]++;
     		}
     		if(*o=='='&&*v=='Y')
     		{
     			count[small]++;
     			count[l]--;
     			count[l+1]++;
     		}
     		if(*o=='='&&*v=='N')
     		{
     			count[l]++;
     			count[l+1]--;
     		}
     	}
     	int ccount=0,mcount=k;
     	for(R::iterator it=count.begin();it!=count.end();it++)
     	{
     		ccount+=it->second;
     		if(it->first<=big)
     			mcount=min(mcount,ccount);
     	}
     	printf("%d\n",mcount);
     }
    	return 0;
    }

