#include <iostream>
using namespace std;

int main() {
    int N;
    cin >> N;
    if (N == 0) {
        cout << 0;
        return 0;
    }
    
    while (N % 10 == 0) {
        N /= 10;
    }
    
    int reversed = 0;
    while (N > 0) {
        reversed = reversed * 10 + (N % 10);
        N /= 10;
    }
    
    cout << reversed;
    
    return 0;
}