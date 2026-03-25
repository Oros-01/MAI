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
    if(Schoko/2 >= k){
        cout << "YES";
    }
    else cout << "NO";

    return 0;
}