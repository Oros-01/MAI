#include <iostream>
using namespace std;

int main(){
    int x;
    int y;
    int x1;
    int y1;
    cin >> x;
    cin >> y;
    cin >> x1;
    cin >> y1;

    if((x < 9 && x > 0) && (y < 9 && y > 0) && (x1 < 9 && x1 > 0) && (y1 < 9 && y1 > 0)){
        if((abs(x-x1) <= 1) && (abs(y-y1) <=1 )){
            cout << "YES";
        }
        else cout << "NO";
    }
    else return 1;
    

    return 0;
}