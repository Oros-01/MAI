#include <iostream>
using namespace std;

int main() {
    int a, b;
    cin >> a >> b;
    
    int idx = (a - b + 10000) / 10001;

    int result[2] = {b, a};

    cout << result[idx] << endl;
    
    return 0;
}