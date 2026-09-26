#include <iostream>
using namespace std;

int main(){
    double a;
    int n;
    double sum = 1;
    double power = 1;
    cin >> a >> n;
    for(int i = 0; i < n; i++){
        power = power * a;
        sum = sum+power;

    }
    cout << sum;
    return 0;
}