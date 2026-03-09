#include <iostream>
using namespace std;

int main(){
    int n = 0;
    cout << "Введите количество секунд:" << endl;
    cin >> n;
    if(n>0 && n < 1000000000){
        int second = n % 60;
        int minute = (n / 60) % 60;
        int hour = (n /3600) % 24;
    
        if(hour < 10){
            cout << "0" << hour << ":";
        }
        else cout << hour<< ":";
        if(minute < 10){
            cout << "0" << minute <<":";
        }
        else cout << minute<< ":";
        if(second < 10){
            cout << "0" << second;
        }
        else cout << second;
    }
    else cout << "НЕВЕРНЫЙ ВВОД!" << endl;

    return 0;
}