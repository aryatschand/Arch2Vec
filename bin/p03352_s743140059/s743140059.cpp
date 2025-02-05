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

#define int long long

signed main(){

	int n;

	cin>>n;

	int count=0;

	int k=sqrt(n);

	int maxi=1;

	for(int i=1; i<n; i++){

		for(int j=2; j<n; j++){

			if(pow(i,j)<=n){

			count=pow(i,j);

			maxi=max(count,maxi);

		//	cout<<i<<" "<<j<<" "<<maxi<<endl;

		}

	}}

	cout<<maxi;

}