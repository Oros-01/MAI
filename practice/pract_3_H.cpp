#include <iostream>
using namespace std;

int main(){
    int num;
    int pred = 0;
    int n = 0;
    cin >> pred;
    cin >> num;
    while(num != 0){
        if(num>pred){
            n++;
        }
        pred = num;
        cin >> num;
    }

    cout << n;

    return 0;
}