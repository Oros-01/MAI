//monkey see, monkey do
#include <iostream>
#include <cmath>
using namespace std;

int main(){
    int x;
    cin >> x;
    int n = 0;
    
    for(int i = 1; i*i <= x; i++){
        if(x%i == 0){ //1,2,4,8,16, 32
            if(i == x/i){
                n++;
            }
            else n+=2;
        }

    }
    cout << n;


    return 0;
}