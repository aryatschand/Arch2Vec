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



int main(){

  int X;

  cin >> X;

  double ans=1;

  for(int i=X;i>0;--i){

    for(int j=1;j<=sqrt(X);++j){

      for(int k=2;k<=10;++k){

        if(pow(j,k)<=X) ans=max(ans,pow(j,k));

      }

    }

  }

  cout << ans << endl;

}