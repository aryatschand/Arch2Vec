#include <iostream>

#include <vector>

#include <algorithm>

#include <string>

#include <cmath>



using namespace std;



int main() {

    

    int X;

    cin >> X;

    

    int ans=1;

    

    for(int b=1;b<=1000;b++){

        for(int p=2;p<=10;p++){

            if((pow(b,p)<=X)&&(pow(b,p)>=ans)){

                ans = pow(b,p);

            }

        }

    }

    

    cout << ans << endl;

    

    return 0;

}