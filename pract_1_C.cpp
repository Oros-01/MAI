#include <iostream>
using namespace std;


int main(){
    int n;
    cin >> n;
    n = n % (24*60);
    int hour = (n/60);
    int minute = n % 60;
    
    cout << hour << " " << minute << endl;

    return 0;
}