#include <iostream>
using namespace std;

int main(){
    int N;
    int cnt = 0;
    cin >> N;
    int arr[N];
    for(int i = 0; i < N; i++){
        cin >> arr[i];
        if(arr[i]>0){
            cnt++;
        }
    }

    cout << cnt;
    return 0;
}