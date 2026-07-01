#include <iostream>
#include <cmath>
using namespace std;
// round - округлить
// pow(x, n) возведение x в n-ю степень!!
int main(){
    int x;
    cin >> x;

    for(int i = 1; i <= x; i++){
        if(x%i == 0){
            cout << i << endl;
        }
    }



    return 0;
}