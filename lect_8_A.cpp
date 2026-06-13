#include <iostream>
using namespace std;


bool power_2(int n) {
    if (n <= 0) {
        return false;
    }
    while (n % 2 == 0) {
        n /= 2;
    }
    return n == 1;
}

int main(){
    int a = 1024;
    int b = 1025;
    cout << power_2(a);
    cout << power_2(b);

}