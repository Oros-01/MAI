#include <iostream>
#include <cmath>
using namespace std;



int calc(int a, int b){
    float result = ((a*a) + (b*b));
    return result;
    }

int main(){
    int a = 0;
    int b = 0;
        
        if (!(cin >> a) || a < 0 || a > 1000){
            return 1;
        }
        
        if (!(cin >> b) || b < 0 || b > 1000){
            return 1;
        }
    cout << sqrt(calc(a,b)) << endl;

    return 0; 
}
