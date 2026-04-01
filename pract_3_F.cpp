#include <iostream>
using namespace std;

int main(){
    double avarage = 0;
    int num;
    int n = 0;
    double sum = 0;

    cin >> num;
    while(num != 0){
        sum = sum + num;
        n++;
        cin >> num;

    }
    if(n>0){
        avarage = sum/n;
        cout << avarage;
        
    }
    else cout << 0;



    return 0;
}