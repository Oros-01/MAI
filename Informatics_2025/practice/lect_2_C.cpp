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
    if((x > 8 || x < 0) || (x1 > 8 || x1 < 0) || (y > 8 || y < 0) || (y1 > 8 || y1 < 0)){
        return 1;
    }
    else if(x == x1 || y == y1){
        cout << "YES";
    }
    else cout << "NO";


    return 0;
}