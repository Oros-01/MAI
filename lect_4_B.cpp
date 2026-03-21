#include <iostream>
#include <cmath>
using namespace std;
// round - округлить
// pow(x, n) возведение x в n-ю степень!!
int main(){
    int a;
    int b;
    cin >> a;
    cin >> b;

    for(int i = a; i <= b; i++){
        double degree = pow(i, 0.5);
        if(degree == round(degree)){
            cout << i << endl;
        }
    }



    return 0;
}