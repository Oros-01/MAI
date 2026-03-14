#include <iostream>
using namespace std;

int main(){
    int year = 0;
    if (!(cin >> year) || year < 0 || year > 30000){
        return 1;
    }
    else if((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)){
        cout << "YES";
    }
    else cout << "NO";


    return 0;
}