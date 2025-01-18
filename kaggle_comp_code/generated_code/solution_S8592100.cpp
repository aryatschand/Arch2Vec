
    #include <string>
    #include <vector>
    #include <algorithm>
    #include <numeric>
    #include <set>
    #include <map>
    #include <queue>
    #include <iostream>
    #include <sstream>
    
    #include <cassert>
    #include <limits>
    #define rep(i,n) for(int (i)=0;(i)<(int)(n);++(i))
    #define reu(i,l,u) for(int (i)=(int)(l);(i)<(int)(u);++(i))
    #define each(it,o) for(auto it= (o).begin(); it != (o).end(); ++ it)
    #define all(o) (o).begin(), (o).end()
    #define mp(x,y) make_pair((x),(y))
    #define mset(m,v) memset(m,v,sizeof(m))
    #define INF 0x3f3f3f3f
    #define INFL 0x3f3f3f3f3f3f3f3fLL
    #define inrep int t;cin>>t; while(t--)
    using namespace std;
    
    typedef vector<int> vi;
    typedef pair<int,int> pii;
    typedef vector<pii > vpii;
    typedef long long ll;
    typedef vector<ll> vll;
    typedef pair<ll,ll> pll;
    typedef vector<pll > vpll;
    typedef vector<string> vs;
    typedef long double ld;
    
    template<typename T> ostream& operator<< ( ostream &o,vector<T> v ) {
        if ( v.size() >0 )
            o<<v[0];
        for ( unsigned   i=1; i<v.size(); i++ )
            o<<" "<<v[i];
        return o<<endl;
    }
    template<typename T> istream& operator>> ( istream &in,vector<T> &v ) {
    
        for ( unsigned   i=0; i<v.size(); i++ )
            in>>v[i];
        return in;
    }
    
    template<typename T> struct DynSegment {
        struct node {
            T val;
            int l=0;
            int r=0;
        };
        const int NODESINCR=3000000;
        vector<node> nodes;
        unsigned _nodecnt=0;
        int newNode() {
    //          assert(_nodecnt==0 || _nodecnt<nodes.size());
            if ( _nodecnt>=nodes.size() ) {
                nodes.resize ( nodes.size() +NODESINCR );
            }
    //         assert ( _nodecnt<nodes.size() );
            return _nodecnt++;
    
        }
        const int Imin,Imax;
        DynSegment ( int mi, int ma ) :Imin ( mi ),Imax ( ma ) {
            newNode();
        }
        void createChildren ( int no ) {
            int n1=newNode(),n2=newNode();
            nodes[no].l=n1;
            nodes[no].r=n2;
        }
        void update ( int no, int L, int R, int l, int r,const T& v ) {
    //         assert ( R>L );
    //         assert ( no<_nodecnt );
    
            if ( l<=L && R<=r ) {
                nodes[no].val.update ( v );
                return;
            }
    
            if ( !nodes[no].l )
                createChildren ( no );
    
            nodes[no].val.updateChildren ( nodes[nodes[no].l].val,nodes[nodes[no].r].val );
    
    
    
            int s= ( L+R ) >>1;
            if ( l<s ) {
                update ( nodes[no].l,L,s,l,r,v );
            }
            if ( r>s ) {
                update ( nodes[no].r,s,R,l,r,v );
            }
            nodes[no].val.merge ( nodes[nodes[no].l].val,nodes[nodes[no].r].val );
    
        }
        void update ( int l, int r,const T&v ) {
            if ( r<=l ) return;
            update ( 0,Imin,Imax,l,r,v );
        }
        T query ( int no, int L, int R, int l, int r ) {
            if ( l<=L && R<=r ) {
                return nodes[no].val.getVal ();
    
            }
            if ( !nodes[no].l )
                createChildren ( no );
            nodes[no].val.updateChildren ( nodes[nodes[no].l].val,nodes[nodes[no].r].val );
            int s= ( L+R ) >>1;
            T res;
            if ( l<s ) {
                T res2=query ( nodes[no].l,L,s,l,r );
                res= res.combine ( res2 );
            }
            if ( r>s  &&nodes[no].r ) {
    
                T res2= query ( nodes[no].r,s,R,l,r );
                res=res.combine ( res2 );
            }
            return res;
        }
        T query ( int l,int r ) {
            if ( r<=l ) return T();
            return query ( 0,Imin,Imax,l,r );
    
        }
    };
    struct nodeval {
        int val=0;
        int incr=0;
        nodeval() {}
        nodeval ( int v ) :val ( v ) {}
        void update ( const nodeval& x ) {
            val+=x.val;
            incr+=x.val;
    
        }
        void reset() {
            val=0;
        }
        void merge ( const nodeval& l, const nodeval &r ) {
            val=min ( l.val,r.val );
            incr=0;
        }
    
        void updateChildren ( nodeval & l,nodeval& r ) {
            if ( !incr ) return;
            l.update ( incr );
            r.update ( incr );
            incr=0;
    
        }
        nodeval getVal() const {
            return *this;
        }
    
        nodeval combine ( const nodeval &other ) const {
    //         assert ( !incr );
            nodeval res;
            res.val=min ( other.val,val );
            return res;
        }
    };
    typedef DynSegment<nodeval> DMinSeg;
    const int MIN=1;
    const int MAX=1000000001;
    int main() {
        ios_base::sync_with_stdio ( false );
        inrep {
            DMinSeg ds ( MIN,MAX );
            int k;
            cin>>k;
    //         assert ( k>0 );
            while ( k-- ) {
                char op;
                int n;
                string c;
                cin>>op>>n>>c;
    //             assert ( n>=MIN && n<MAX );
    //             assert ( c=="No" || c=="Yes" );
    
                switch ( op ) {
                case '<':
                    if ( c=="Yes" )
                        ds.update ( n,MAX,1 );
                    else ds.update ( MIN,n,1 );
                    break;
                case '>':
                    if ( c=="Yes" )
                        ds.update ( MIN,n+1,1 );
                    else ds.update ( n+1,MAX,1 );
                    break;
                case '=':
                    if ( c=="Yes" ) {
                        ds.update ( MIN,n,1 );
                        ds.update ( n+1,MAX,1 );
                    } else ds.update ( n,n+1,1 );
                    break;
                default:
                    assert ( 0 );
                }
            }
    //         cout<<ds._nodecnt<<endl;
            int res=ds.query ( MIN,MAX ).val;
            cout<<res<<endl;
    
    
        }
    }
    

