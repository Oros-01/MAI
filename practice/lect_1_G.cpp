#include <iostream>
using namespace std;

int main(){
    int number;
    cin >> number;
    int firstChar = number % 10;
    int secondChar = number % 100;
    int temple = secondChar - firstChar;
    int result = temple / 10;
    cout << result;


    return 0;
}