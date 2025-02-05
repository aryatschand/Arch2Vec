#include <iostream>

#include <vector>

#include <algorithm>

#include <string>

#include <cmath>

#include <set>

#include <sstream>

#include <bitset>



#define FOR(i,a,b) for(int i=(a);i<(b);++i)

#define REP(i,n)  FOR(i,0,n)



typedef long long ll;



using namespace std;



int main() {

    

    int x;

    cin >> x;

    int ans=1;

    FOR(b, 2, 32){

        FOR(p, 2, 10){

            if((pow(b, p)>ans)&&(pow(b, p)<=x)){

                ans = pow(b, p);

            }

        }

    }

    

    cout << ans << endl;

    

    

    return 0;

}
