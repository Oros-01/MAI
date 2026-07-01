#include <iostream>
using namespace std;

bool isEven(int number){
    return number % 2 == 0;
}

void printEven(int smol, int big){

    for(int i = smol; i <= big; i++) {
        if(isEven(i)){
            cout << i << " ";
        }
    }
}


int main(){
    int a = 0;
    int b = 0;

    if(!(cin >> a) || a < -1000 || a > 1000){
        return 1;
    }
    
    if(!(cin >> b) || b < -1000 || b > 1000){
        return 1;
    }
    printEven(a,b);

    return 0;
}