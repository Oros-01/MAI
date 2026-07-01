#include <iostream>
using namespace std;




int main() {
    char s[1001];
    cin >> s;
    
    char maxDigit = '0';
    
    for(int i = 0; s[i] != '\0'; i++) {
        if(s[i] > maxDigit) {
            maxDigit = s[i];
        }
    }
    
    cout << maxDigit << endl;
    
    return 0;
}