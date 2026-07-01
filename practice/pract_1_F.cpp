#include <iostream>
using namespace std;



int main(){
    int a = 0;
    int b = 0;
    int temple1 = 0;
    int temple2 = 0;
    cin >> a;
    cin >> b;
    temple1 = a;
    temple2 = b;
    a = temple2;
    b = temple1;
    cout << a << " " << b << endl;
}