#include <iostream>
using namespace std;

int main(){
    double a;
    int n;
    double sum = 1.0;
    cin >> a >> n;
    while(n > 0){
       if (n % 2 == 1) {
            sum *= a;
        }
        a *= a;
        n /= 2;
    }

    cout << sum;

    return 0;
}