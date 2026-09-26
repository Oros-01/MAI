#include <iostream>
using namespace std;

void reversePrint() {
    int n;
    cin >> n;
    
    if (n != 0) {
        reversePrint();
        cout << n << " ";
    }
}

int main() {
    reversePrint();
    cout << endl;
    return 0;
}