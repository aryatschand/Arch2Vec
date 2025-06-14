
    //#undef _GLIBCXX_DEBUG
    
    #include <algorithm>
    #include <cassert>
    #include <cmath>
    #include <cstdio>
    #include <cstring>
    #include <ctime>
    #include <iomanip>
    #include <iostream>
    #include <limits>
    #include <list>
    #include <map>
    #include <numeric>
    #include <queue>
    #include <set>
    #include <sstream>
    #include <valarray>
    #include <vector>
    
    using namespace std;
    
    #define foreach(iter,cont) for(typeof((cont).begin()) iter = (cont).begin(); iter != (cont).end(); ++iter)
    #define min3(a, b, c) min(a, min(b, c))
    #define max3(a, b, c) max(a, max(b, c))
    #define inf 1000000008
    #define eps 1.0e-10L
    #define divisible(a, b) (((a) / (b)) * (b) == (a))
    //static const long long p = 1000000007;
    __attribute__((unused)) static bool eq(long double a, long double b)  { return (a == b ? true : (a * b == 0 ? abs(a - b) < eps : abs(a - b) / (abs(a) + abs(b)) < eps));  }
    __attribute__((unused)) static long long modp(long long n, long long m) { long long r = n % m; return r >= 0 ? r : r + m; }
    __attribute__((unused)) static long long gcd(long long a, long long b) { return (a < 0 ? gcd(-a, b) : (b < 0 ? gcd(a, -b) : (!a && !b ? 1 : (b ? gcd(b, a % b) : a)))); }
    template<class T> static int len(const T& cont) { return (int) cont.size(); }
    
    // BEGIN CUT HERE
    template<class T, class U> ostream &operator<<(ostream &os, const pair<T, U> &p) { return os << "(" << p.first << ", " << p.second << ")"; }
    template<class T> ostream &operator<<(ostream& os, const vector<T> &vec) { os << "[ "; for(size_t i = 0; i < vec.size(); i++) { os << vec[i]; if(i != vec.size() - 1) { os << ", "; } } return os << " ]"; }
    template<class T> ostream &operator<<(ostream& os, const list<T> &l) { os << "[ "; bool first = true; for(typename list<T>::const_iterator lstIt = l.begin(); lstIt != l.end(); ++lstIt) { if(!first) { os << ", "; } first = false; os << *lstIt; } return os << " ]"; }
    template<class T> ostream &operator<<(ostream& os, const deque<T> &d) { os << "[ "; bool first = true; for(typename deque<T>::const_iterator dIt = d.begin(); dIt != d.end(); ++dIt) { if(!first) { os << ", "; } first = false; os << *dIt; } return os << " ]"; }
    template<class T> ostream &operator<<(ostream& os, const vector<vector<T> > &mat) { for(size_t i = 0; i < mat.size(); i++) { if(i) os << endl; os << mat[i]; } return os; }
    template<class T> ostream &operator<<(ostream &os, const set<T> &s) { os << "{ "; bool first = true; for(typename set<T>::const_iterator setIt = s.begin(); setIt != s.end(); ++setIt) { if(!first) { os << ", "; } first = false; os << *setIt; } return os << " }"; }
    template<class T, class U> ostream &operator<<(ostream &os, const map<T, U> &m) { os << "{ "; bool first = true; for(typename map<T, U>::const_iterator mapIt = m.begin(); mapIt != m.end(); ++mapIt) { if(!first) { os << ", "; } first = false; os << mapIt->first << " = " << mapIt->second; } return os << " }"; }
    enum CONSOLE_FG { FG_BLACK, FG_RED, FG_GREEN, FG_YELLOW, FG_BLUE, FG_MAGENTA, FG_CYAN, FG_WHITE };
    enum CONSOLE_STYLE { STYLE_NONE, STYLE_BOLD };
    ostream& operator<<(ostream &os, CONSOLE_FG color) { return os << "\x1b[" << ((int) color + 30) << "m"; }
    ostream& operator<<(ostream &os, CONSOLE_STYLE style) { return os << "\x1b[" << (style == STYLE_NONE ? 0 : 1) << "m"; }
    // END CUT HERE
    
    int t, k, m;
    char op[2], yesNo[4];
    map<int, int> ranges;
    
    int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
    	scanf("%d", &t);
    	while(t--) {
    		scanf("%d", &k);
    
    		ranges.clear();
    
    		for(int i = 0; i < k; i++) {
    			scanf("%s %d %s", op, &m, yesNo);
    
    			if(op[0] == '<' && yesNo[0] == 'Y') {
    				ranges[1]++;
    				ranges[m]--;
    			} else if(op[0] == '<' && yesNo[0] == 'N') {
    				ranges[m]++;
    				ranges[1000000001]--;
    			} else if(op[0] == '>' && yesNo[0] == 'Y') {
    				ranges[m + 1]++;
    				ranges[1000000001]--;
    			} else if(op[0] == '>' && yesNo[0] == 'N') {
    				ranges[1]++;
    				ranges[m + 1]--;
    			} else if(op[0] == '=' && yesNo[0] == 'Y') {
    				ranges[m]++;
    				ranges[m + 1]--;
    			} else if(op[0] == '=' && yesNo[0] == 'N') {
    				ranges[1]++;
    				ranges[m]--;
    				ranges[m + 1]++;
    				ranges[1000000001]--;
    			}
    		}
    		
    		int maxVal = 0, curVal = 0;
    		foreach(r, ranges) {
    			curVal += r->second;
    
    			if(r->first >= 1 && r->first <= 1000000000) {
    				maxVal = max(maxVal, curVal);
    			}
    		}
    
    		printf("%d\n", k - maxVal);
    	}
    	
    	return 0;
    }
    

