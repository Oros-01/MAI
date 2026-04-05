#include <iostream>
using namespace std;

int main(){
    int cnt = 0;
    int n;
    int num;
    cin >> n;
    for(int i = 0; i < n; i++){
        cin >> num;
        if(num == 0){
            cnt++;

        }
    }
    cout << cnt;

    return 0;
}