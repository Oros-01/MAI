#include <iostream>
using namespace std;

int main(){
    int minute = 0;
    int hour = (minute * 60);
    int day = (hour * 60);
    cin >> minute;
    while(minute > 60){
        minute = minute-60;
        hour++;
        if(hour > 23){
            hour = hour-24;
            day++;
        }
    }
    if(day < 10){
        cout << "0" << day << ":" << hour << ":" << minute << endl;
    }
    else if(day < 10 && hour < 10){
        cout << "0" << day << ":" << "0" << hour << ":" << minute << endl;
    }
    else{
        cout << "0" << day << ":" << "0" << hour << ":" << "0" << minute << endl;
    }
    return 0;
}