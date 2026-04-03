#include <iostream>
using namespace std;

int main(){
    int num;
    int max = 0;
    int n = 0;
    cin >> num;
    while(num != 0){
        if(num>max){
            max = num;
            n = 1;
        }
        else if(num == max){
            n++;
        }
        cin >> num;
    }
    cout << n;
    return 0;
}