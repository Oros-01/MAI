#include <iostream>
using namespace std;



int main(){
    int kids = 0;
    int apples = 0;
    cin >> kids;
    cin >> apples;
    int result = apples % kids;
    cout << result;

    return 0;
}