
    #include <stdio.h>
    #include <string>
    #include <math.h>
    #include <fstream>
    #include <iostream>
    #include <vector>
    #include <deque>
    #include <map>
    #include <list>
    #include <cstdlib>
    using namespace std;
    
    //2009 March Long Contest, MARCHA3 The Guessing Game codechef.com
    //Disable warning messages C4996.
    #pragma warning(disable:4996)
    
    /*#define INPUT_FROM 0
    #if INPUT_FROM
    #define fromc from
    #else
    #define fromc cin
    #endif*/
    
    //long long fr(vector<long long>&, long long);
    
    int main(int argc, char **argv)
    {
    	//ifstream from;
    	const int nmin=1, nmax=1000000000;
    	int test, cases, n, m, mt, res, i, j, k, t, rt, ax;
    	int x, y, m0, m1;
    	char ch;
    	char str[4];
    	//string sres[2]={"YES", "NO"};
    	//string s, st, sr;
    	//long double dt;
    
    	//if(INPUT_FROM) from.open("test.txt");
    	//freopen("input.txt","r",stdin);
    	//freopen("output.txt","w",stdout);
    
    	//static int va[nmax+1];
    	//int* pv;
    	//static long long v[nmax+1];
    	//vector<long long> v;
    	//static long long vr[qmax];
    	//deque<long long> dq;
    	map<int, int> mi, mi0, mi1;
    	map<int, int>::iterator it, it0, it1;
    	//typedef map<string, long long>::const_iterator CI;
    
    
    	scanf("%ld\n", &cases);
    	for(test=1;test<=cases;test++){
    		scanf("%ld\n", &n);
    
    		for(i=0;i<n;i++){
    			scanf("%c %ld %s\n", &ch, &x, str);
    			if(str[0]=='N'){
    				if(ch=='='){
    					if((x>nmin)&&(mi0.find(x-1)==mi0.end())) mi0[x-1]=0;
    					if(mi0.find(x)==mi0.end()) mi0[x]=1;
    					else mi0[x]++;
    					if(x<nmax){
    						if(mi0.find(x+1)==mi0.end()) mi0[x+1]=-1;
    						else mi0[x+1]--;
    					}
    				}else if((ch=='>')&&(x<nmax)){
    					if(mi0.find(x)==mi0.end()) mi0[x]=0;
    					if(mi0.find(x+1)==mi0.end()) mi0[x+1]=1;
    					else mi0[x+1]++;
    				}else if((ch=='<')&&(x>nmin)){
    					if(mi1.find(x)==mi1.end()) mi1[x]=0;
    					if(mi1.find(x-1)==mi1.end()) mi1[x-1]=1;
    					else mi1[x-1]++;
    				}
    			}else{
    				if(ch=='='){
    					if(mi0.find(x)==mi0.end()) mi0[x]=0;
    					if(x<nmax){
    						if(mi0.find(x+1)==mi0.end()) mi0[x+1]=1;
    						else mi0[x+1]++;
    					}
    					if(x>nmin){
    						if(mi1.find(x-1)==mi1.end()) mi1[x-1]=1;
    						else mi1[x-1]++;	
    					}
    				}else if(ch=='>'){
    					if((x<nmax)&&(mi1.find(x+1)==mi1.end())) mi1[x+1]=0;
    					if(mi1.find(x)==mi1.end()) mi1[x]=1;
    					else mi1[x]++;
    				}else if(ch=='<'){
    					if((x>nmin)&&(mi0.find(x-1)==mi0.end())) mi0[x-1]=0;
    					if(mi0.find(x)==mi0.end()) mi0[x]=1;
    					else mi0[x]++;
    				}			
    			}
    		}
    
    		m0=mi0.size();
    		m1=mi1.size();
    
    		if(m0>0){
    			for(it=mi0.begin();it!=mi0.end();it++) mi[it->first]=0;
    			x=mi0.begin()->first;
    			if(x>nmin) mi[x-1]=0;
    		}
    		if(m1>0){
    			for(it=mi1.begin();it!=mi1.end();it++) mi[it->first]=0;
    			it=mi1.end();
    			it--;
    			x=it->first;
    			if(x<nmax) mi[x+1]=0;
    		}
    
    		if(mi.size()==0) res=0;
    		else res=n;
    
    		it=mi.begin();
    		rt=0;
    		for(it0=mi0.begin();it0!=mi0.end();it0++){
    			x=it0->first;
    			while((it->first)!=x){
    				(it->second)+=rt;
    				if(!m1){y=(it->second); if(y<res) res=y;}
    				it++;
    			}
    			rt+=it0->second;
    			(it->second)+=rt;
    			if(!m1){y=(it->second); if(y<res) res=y;}
    			it++;
    		}
    		while(it!=mi.end()){
    				(it->second)+=rt;
    				if(!m1){y=(it->second); if(y<res) res=y;}
    				it++;
    		}
    
    		if(m1>0){
    			rt=0;
    			it=mi.end();
    			it1=mi1.end();
    			do{
    				it--; it1--;
    				x=it1->first;
    				while((it->first)!=x){
    					(it->second)+=rt;
    					y=(it->second); if(y<res) res=y;
    					it--;
    				}
    				rt+=it1->second;
    				(it->second)+=rt;
    				y=(it->second); if(y<res) res=y;
    			}while(it1!=mi1.begin());
    
    			while(it!=mi.begin()){
    				it--;
    				(it->second)+=rt;
    				y=(it->second); if(y<res) res=y;
    			}
    		}
    
    		printf("%ld\n", res);
    		//printf("%c %ld %s\n", ch, x, str);
    		mi.clear();
    		mi0.clear();
    		mi1.clear();
    	}
    
    	return 0;
    }
    
    
    

