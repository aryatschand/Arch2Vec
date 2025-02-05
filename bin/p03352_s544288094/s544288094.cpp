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

const int INF=1001001001;

vector<pair<int64_t,int64_t>>prime_factorize(int64_t x){

  vector<pair<int64_t,int64_t>>p;

  for(int64_t i=2;i*i<=x;i++){

    int cnt=0;

    if(x%i==0){

    while(x%i==0){cnt++;x/=i;}

    p.push_back(make_pair(i,cnt));

    }

  }

  if(x!=1){p.push_back(make_pair(x,1));}

  return p; 

}



int main() {

  int X;

  cin>>X;

  int ans=1;

  for(int b=1;b<=X;b++){

    for(int p=2;p<=X;p++){

      int s=pow(b,p);

      if(s<=X){chmax(ans,s);}

    }

  }

  cout<<ans<<endl;

  return 0;

}