#include <iostream>
using namespace std;

int main(){
    int size;
    int cnt = 1;
    cin >> size;
    int arr[size];
    for(int i = 0; i < size; i++){
        cin >> arr[i];
    }
    for(int i = 1; i < size; i++){
        if(arr[i-1] != arr[i]){
            cnt++;
        }
    }
    cout << cnt;

    return 0;
}