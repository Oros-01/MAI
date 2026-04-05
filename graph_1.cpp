/*
Область определения:
y от -2 до 0
y = x
y =-x
*/


#include <iostream>
using namespace std;


int main(){
    int x = 0;
    int y = 0;
    cin >> y;
    cin >> x;
    if (y >= x*x -2){
        if (y <= x){
            if (y <= -x){
                cout << "YES";
            }
            else {
                cout << "NO";
            }
        }
    }
    return 0;
}



