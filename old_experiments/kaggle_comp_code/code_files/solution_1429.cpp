
    #include <iostream>
    #include<stdlib.h>
    #include<stdio.h>
    #include<map>
    #define si(a) scanf("%d",&a)
    #define sf(a) scanf("%f",&a)
    #define sc(a) scanf("%c",&a)
    #define ss(a) scanf("%s",a)
    #define small 1
    #define big 1000000000
    using namespace std;
    
    
    int main()
    {
        typedef map <int,int> K;
        K count;
        int t;
        si(t);
        while(t--)
        {
            int k;
            si(k);
            count.clear();
                count[small]=0;
            for(int i=0;i<k;i++)
            {   int q;
    
                char op[2],logic[4];
                ss(op);
                si(q);
                ss(logic);
    
                /*cout<<op<<" "<<l<<" "<<logic;*/
    
                if(*op=='<' && *logic=='Y')
                {
                    count[q]++;
                }
                if(*op=='<' && *logic=='N')
                {
                    count[q]--;
                    count[small]++;
                }
                if(*op=='>' && *logic=='Y')
                {
                    count[small]++;
                    count[q+1]--;
    
                }
                if(*op=='>' && *logic=='N')
                {
                    count[q+1]++;
    
                }
                if(*op=='=' && *logic=='Y')
                {
                    count[small]++;
                    count[q]--;
                    count[q+1]++;
    
                }
                if(*op=='=' && *logic=='N')
                {
                    count[q]++;
                    count[q+1]--;
    
                }
    
    
            }
    
            int counts=0,mincount=k;
            for(K::iterator i=count.begin();i!=count.end();i++)
            {
                counts+=i->second;
                if(i->first<=big)
                    mincount=min(mincount,counts);
    
            }
    
            printf("%d\n",mincount);
    
        }
    
    
    
        return 0;
    }
    

