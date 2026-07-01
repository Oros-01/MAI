#include <iostream>
using namespace std;
#include <cmath>




int main(){
int x;
int y;
int x1;
int y1;
cin >> x;
cin >> y;
cin >> x1;
cin >> y1;
if((0 < x && x < 9) && (0 < y && y < 9) && (0 < x1 && x1 < 9) && (0 < y1 && y1 < 9)){
    
    if(x == x1 || y == y1 || abs(x-x1) == abs(y-y1)){
        cout << "YES" << endl;
    }
    else cout << "NO";

}
else cout << "НЕВЕРНЫЙ ВВОД" << endl;
}