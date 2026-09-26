#include <iostream>
using namespace std;

int main(){
    int num;
    cin >> num;
    int result = 0;
    
    while(num != 0){
        if(num%2 == 0){
            result++;
        }
        cin >> num;
        


    }
    cout << result;


    return 0;
}