#include <iostream>
using namespace std;

int main(){

    int n;
    cin >> n;
    int factN = 1;

    for(int i = 2; i <= n; i++){
        factN = i*factN;

    }
    
    int k;
    cin >> k;
    int factK = 1;

    for(int i = 2; i <= k; i++){
        factK = i*factK;

    }

    int a = n;
    int b = k;
    int factab = 1;

    for(int i = 2; i <= (a-b); i++){
        factab = i*factab;

    }
    int result = factN/(factK*factab);
    
    cout << result;
    

    return 0;
}