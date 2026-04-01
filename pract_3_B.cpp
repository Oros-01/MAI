#include <iostream>
using namespace std;

int main(){
    double x;
    double y;
    cin >> x;
    cin >> y;

    int day = 1;
    double distance = x;

    while(distance < y){
        distance = distance*1.10;
        day++;
    }

    cout << day << endl;

    return 0;

}