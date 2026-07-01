#include <iostream>
using namespace std;

int main() {
    int N;
    cin >> N;
    
    bool isPower = false;
    
    if (N == 1) {
        isPower = true;
    } else {
        int temp = N;
        while (temp % 2 == 0 && temp > 1) {
            temp /= 2;
        }
        if (temp == 1) {
            isPower = true;
        }
    }
    
    if (isPower) {
        cout << "YES" << endl;
    } else {
        cout << "NO" << endl;
    }
    
    return 0;
}