#include <iostream>
#include <string>
#include <algorithm>
using namespace std;



string toBinary(int n) {
    if (n == 0) return "0";
    
    bool isNegative = (n < 0);
    if (isNegative) n = -n;
    
    string binary = "";
    
    while (n > 0) {
        binary += (n % 2) + '0';
        n /= 2;
    }
    
    reverse(binary.begin(), binary.end());
    
    if (isNegative) binary = "-" + binary;
    
    return binary;
}






int main() {
    int N;
    cin >> N;
    
    cout << toBinary(N) << endl;
    
    return 0;
}