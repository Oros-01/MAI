#include <iostream>
using namespace std;

int main(){
    int number;
    cin >> number;
    int firstChar = number % 10;
    int secondChar = number % 100;
    int thirdChar = number % 1000; // 
    int temple1 = thirdChar - secondChar;
    int temple2 = secondChar - firstChar;
    int res1 = firstChar;
    int res2 = temple2 / 10;
    int res3 = temple1 / 100; // получаем третий знак
    int sum = res1+res2+res3;
    cout << res1 << endl;
    cout << res2 << endl;
    cout << res3 << endl;
    cout << sum << endl;

    return 0;
}