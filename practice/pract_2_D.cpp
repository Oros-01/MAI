#include <iostream>
using namespace std;

int main(){

    int n;
    int m;
    int k;

    cin >> n;
    cin >> m;
    cin >> k;

    int Schoko = n*m;
    if((k % n == 0 || k % m == 0) && k < n * m){
        cout << "YES";
    }
    else cout << "NO";

    return 0;
}