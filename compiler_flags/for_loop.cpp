#include <iostream>
#include <Eigen/Dense>
Eigen::Matrix<Eigen::Matrix<double, 16, 1>,6,6> var0;
void foo(){
  Eigen::Matrix<Eigen::Matrix<double, 16, 1>,6,6> var1;
  Eigen::Matrix<Eigen::Matrix<double, 16, 1>,6,6> var2;
  for (int i=0; i < 6; i = i+1) {
        for (int j=0; j < 6; j = j+1) {
            var0(i,j).array()=0;
            for(int k=0;k < 6 ;++k){
                var0(i,j).array()+=var2(i,k).array()*var1(k,j).array();
            }
        }
  }
}
int main() {
    for (int i = 0; i < 1000000; i++)
        foo();
}