#include <iostream>
using namespace std;

int main(){
    int v = 0;
    int t = 0;
    cin >> v;
    cin >> t;
    int distance = v*t;
    int result = distance % 109;
    
    if (result > 108){
        result = result - 109;
        cout << result;
    }
    else if(result < 0){
        result = result + 109;
        cout << result;
    }
    else cout << result;

   

    return 0;
}