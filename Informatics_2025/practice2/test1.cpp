#include <iostream>

using namespace std;

void foo(int a){
    a = 0;
}

void boo(int& a){
    a = 20;
}

void doo(int* a){
    *a = 30;
}
int main(){

    int a = 10;
    cout << a << endl;
    foo(a);
    cout << a << endl;
    boo(a);
    cout << a << endl;
    doo(a);
    cout << a << endl;



    return 0;
}