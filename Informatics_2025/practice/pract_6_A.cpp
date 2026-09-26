#include <iostream>
using namespace std;

int main(){
    int size;
    int cnt = 0;
    cin >> size;
    int arr[size];
    for(int i = 0; i < size; i++){
        cin >> arr[i];
    }
    for(int i = 1; i < size-1 ; i++){
        if(arr[i] > arr[i-1] && arr[i] > arr[i+1]){
            cnt++;
            
        }
    }
    cout << cnt;
    return 0;
}