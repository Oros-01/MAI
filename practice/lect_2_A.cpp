#include <iostream>
using namespace std;

int larger_number(int a, int b){
    if (a > b){
        return a;
    }
    else if(a < b) {
        return b;
    }
    }

int main(){
    int a = 0;
    int b = 0;
    
    if (!(cin >> a) || a > 1000 || a < -1000){
        return 1;
        }
    if (!(cin >> b) || b > 1000 || b < -1000){
        return 1;
        }

    cout << larger_number(a,b) << endl;


    return 0;
}