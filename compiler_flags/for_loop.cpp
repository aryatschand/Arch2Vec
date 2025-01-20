#include <iostream>
#include <Eigen/Dense>
Eigen::Matrix<Eigen::Matrix<double, 16, 1>,32,32> var0;
void foo(){
  Eigen::Matrix<Eigen::Matrix<double, 16, 1>,32,32> var1;
  Eigen::Matrix<Eigen::Matrix<double, 16, 1>,32,32> var2;
  for (int i=0; i < 32; i = i+1) {
        for (int j=0; j < 32; j = j+1) {
            var0(i,j).array()=0;
            for(int k=0;k < 32 ;++k){
                var0(i,j).array()+=var2(i,k).array()*var1(k,j).array();
            }
        }
  }
}
int main() {
    for (int i = 0; i < 100000; i++)
        foo();
}