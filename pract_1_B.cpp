#include <iostream>
using namespace std;

int main() {
    int rub, kop, n;
    cin >> rub >> kop >> n;
    int totalKop = (rub * 100 + kop) * n;
    int resultRub = totalKop / 100;
    int resultKop = totalKop % 100;
    
    cout << resultRub << " " << resultKop << endl;
    
    return 0;
}