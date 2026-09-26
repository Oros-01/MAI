#include <iostream>
using namespace std;

int main(){
    int n = 0;
    cin >> n;
    if(n >= 0 && n < 1000000000){
        int second = n % 60;
        int minute = (n / 60) % 60;
        int hour = (n /3600) % 24;
        cout << hour << ":";
        if(minute < 10){
            cout << "0" << minute <<":";
        }
        else cout << minute<< ":";
        if(second < 10){
            cout << "0" << second;
        }
        else cout << second;
    }

    return 0;
}