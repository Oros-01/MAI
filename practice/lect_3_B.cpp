#include <iostream>
using namespace std;

int main() {
    int N;
    cin >> N;
    
    int divisor = 2;
    for(int i = 2; i <= N; i++) {
        if(N % i == 0) {
            divisor = i;
            break;
        }
    }
    
    cout << divisor << endl;
    
    return 0;
}