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
        }
    }
    cout << hour << ":" << minute << endl;

    return 0;
}