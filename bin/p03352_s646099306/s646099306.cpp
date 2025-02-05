#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <set>
#include <map>
#include <queue>
#include <stack>
#include <bitset>
using namespace std;

using namespace std;

#include <math.h>

#include <iomanip>

template<class T> inline bool chmax(T& a, T b) { if (a < b) { a = b; return 1; } return 0; }

template<class T> inline bool chmin(T& a, T b) { if (a > b) { a = b; return 1; } return 0; }



const int mod=1e9+7;

const int INF=1001001001;



int main() {

  int x;

  cin>>x;

  int MAX=1;

  for(int i=2;i*i<=x;i++){

    int p=i;

    while(p*i<=x){

      p*=i;

    }

    chmax(MAX,p);

  }

  cout<<MAX<<endl;

  return 0;  

}