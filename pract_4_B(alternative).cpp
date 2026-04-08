#include <iostream>
using namespace std;

int main(){
    int n;
    int cnt = 0;
    cin >> n;
    int num;
    for(int i = 0; i < n; i++){
        cin >> num;
        if(num == 0){
            cnt++;
        }

    }
    cout << cnt;

    return 0;
}