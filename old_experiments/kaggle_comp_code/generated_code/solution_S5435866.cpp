
    #include <algorithm>
    #include <iostream>
    #include <fstream>
    #include <cstdlib>
    #include <cstring>
    #include <cmath>
    #include <deque>
    #include <vector>
    #include <queue>
    #include <map>
    
    using namespace std;
    
    #define X first
    #define Y second
    #define INPUT freopen(".inp","r",stdin)
    #define OUTPUT freopen(".out","w",stdout)
    #define FOR(i,l,r) for(int i=l;i<=r;i++)
    #define REP(i,l,r) for(int i=l;i<r;i++)
    #define FORD(i,l,r) for(int i=l;i>=r;i--)
    #define REPD(i,l,r) for(int i=l;i>r;i--)
    #define ENDL printf("\n")
    
    typedef long long ll;
    typedef pair<int,int> ii;
    
    const int inf=2e9;
    const int MOD=1e9+7;
    const int N=2e5+100;
    
    map <int,int> ma;
    int n,x,m,f[N],d[N],test;
    string s;
    char ch;
    struct an{
        int sign,x,r;
        an(int _sign,int _x,int _r){
            sign=_sign,x=_x,r=_r;
        }
    };
    vector <an> a;
    void prepare(){
        ma.clear();
        a.clear();
        cin>>n;
        FOR(i,1,n){
            cin>>ch>>x>>s;
            a.push_back(an(((ch=='<')?-1:((ch=='=')?0:1)),x,s[0]=='Y'));
            d[i]=x;
        }
        sort(d+1,d+n+1);
        m=-1;
        int last=0;
        FOR(i,1,n){
            if (d[i]==last+1) m++;
            else m+=2;
            ma[d[i]]=m;
            last=d[i];
            while (i<n&&d[i+1]==d[i]) i++;
        }
        //REP(i,0,n) printf("%d %d %d\n",a[i].sign,ma[a[i].x],a[i].r);
    }
    void solve(){
        memset(f,0,sizeof(f));
        REP(i,0,n){
            int pos=ma[a[i].x];
            if (a[i].sign==0){
                if (a[i].r) f[0]++,f[pos]--,f[pos+1]++;
                else f[pos]++,f[pos+1]--;
            }else if (a[i].sign==-1){
                if (a[i].r) f[pos]++;else f[0]++,f[pos]--;
            }else {
                if (a[i].r) f[0]++,f[pos+1]--;else f[pos+1]++;
            }
        }
        int ans=n+1;
        FOR(i,0,m+(d[n]<1000000000)) {
            if (i>0) f[i]+=f[i-1];
            //printf("%d ",f[i]);
            ans=min(ans,f[i]);
        }
        cout<<ans<<'\n';
    }
    int main(){
        cin>>test;
        while (test--){
            prepare();
            solve();
        }
    }
    

