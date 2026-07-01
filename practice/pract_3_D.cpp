#include <iostream>
using namespace std;

int main(){
    int n = 0;
    int num;
    cin >> num;


    while(num>0){
        n++;
        cin >> num;
    }
    cout << n;


    return 0;
}